#include <iostream>
#include <fstream>
#include <chrono>

#include "Objet/Rayon3D.h"
#include "Objet/Vec3D.h"
#include "Objet/Intersection3D.h"
#include "Objet/Objet3D.h"
#include "Objet/PointLight.h"
#include "Objet/MaterialLibrary.h"
#include "Objet/Scene.h"

#define WIDTH 1920
#define HEIGHT 1080
#define SAMPLES_PER_PIXEL 9

// Kernel
__global__ void render(Vec3D* frameBuffer, int max_x, int max_y, Scene* scene, 
                       Vec3D lower_left, Vec3D horizontal, Vec3D vertical, Vec3D camera_pos)
{
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    int j = blockIdx.y * blockDim.y + threadIdx.y;

    if(i >= max_x || j >= max_y) return;

    Vec3D pixel_color(0.0f, 0.0f, 0.0f);
    // Lancer de rayon
    for(int k = 0; k < SAMPLES_PER_PIXEL; k++){
        // On ajoute un décalage pseudo-aléatoire pour l'antialiasing
        // (Ici un décalage simple pour l'exemple)
        const int N = (int)ceilf(sqrtf((float)SAMPLES_PER_PIXEL));
        int col = k % N;
        int row = k / N;
        float offset_u = (float)col / (float)N;
        float offset_v = (float)row / (float)N;

        float u = (float(i) + offset_u) / float(max_x);
        float v = (float(j) + offset_v) / float(max_y);
        
        Vec3D direction = lower_left + horizontal*u + vertical*v - camera_pos;
        Rayon3D ray(camera_pos, direction);
        
        // On accumule la couleur (castRayIterative doit renvoyer du 0.0 à 1.0)
        pixel_color = pixel_color + scene->castRayIterative(ray, 5); 
    }
    Vec3D color = pixel_color / (float)SAMPLES_PER_PIXEL;
    color.x = powf(fmax(color.x,0.0f),1.0f/2.2f);
    color.y = powf(fmax(color.y,0.0f),1.0f/2.2f);
    color.z = powf(fmax(color.z,0.0f),1.0f/2.2f);

    frameBuffer[j * max_x + i] = color;
}


int main() {
    int nx = WIDTH;
    int ny = HEIGHT;
    int num_pixels = nx * ny;
    float ms = 0.0f;

    Vec3D* frameBuffer;
    cudaMallocManaged((void **)&frameBuffer, num_pixels * sizeof(Vec3D));

    // --- CRÉATION DE LA SCÈNE ---
    Scene* scene;
    cudaMallocManaged((void**)&scene, sizeof(Scene));
    
    // On alloue de l'espace pour 150 objets et 5 lumières
    cudaMallocManaged((void**)&scene->objects, 150 * sizeof(Objet3D));
    cudaMallocManaged((void**)&scene->lights, 5 * sizeof(PointLight));

    scene->num_objects = 0;
    scene->num_lights = 0;

    // Lambdas
    auto addObjet = [&](const Objet3D& obj) { scene->objects[scene->num_objects++] = obj; };
    auto addLight = [&](const PointLight& l) { scene->lights[scene->num_lights++] = l; };

    // Sphère
    addObjet({ SPHERE, Vec3D(-1.2f, 0.0f, -2.0f), Vec3D(0,0,0), 0.25f, MatLib::Red() });
    addObjet({ SPHERE, Vec3D(-0.6f, 0.0f, -2.0f), Vec3D(0,0,0), 0.25f, MatLib::Chrome() });
    addObjet({ SPHERE, Vec3D( 0.0f, 0.0f, -2.0f), Vec3D(0,0,0), 0.25f, MatLib::Gold() });
    addObjet({ SPHERE, Vec3D( 0.6f, 0.0f, -2.0f), Vec3D(0,0,0), 0.25f, MatLib::Steel() });
    addObjet({ SPHERE, Vec3D( 1.2f, 0.0f, -2.0f), Vec3D(0,0,0), 0.25f, MatLib::BlueGlass() });

    for (int i = 0; i < 100; i++) {
        float rx = -30.0f + (float)rand() / (RAND_MAX / 60.0f);
        float ry = -10.0f + (float)rand() / (RAND_MAX / 40.0f);
        float rz = -5.0f - (float)rand() / (RAND_MAX / 45.0f);
        addObjet({ SPHERE, Vec3D(rx, ry, rz), Vec3D(0,0,0), 0.05f, MatLib::Star_mat() });
    }

    // Cube
    addObjet({ CUBE, Vec3D(0.0f, -0.25f, -2.0f), Vec3D(10.0f, 0.01f, 10.0f), 0, MatLib::Floor() });

    // Lumière
    addLight(PointLight(Vec3D(3.0f, 1.0f, 1.0f), Vec3D(1.0f, 1.0f, 1.0f), 0.5f));

    // Caméra
    float aspect_ratio = float(nx) / float(ny);
    float focal = 2.0f;
    float viewport_height = 2.0f;
    float viewport_width = aspect_ratio * viewport_height;
    
    Vec3D origin(0.0f, 0.0f, 0.0f);
    Vec3D horizontal(viewport_width, 0.0f, 0.0f);
    Vec3D vertical(0.0f, viewport_height, 0.0f);
    Vec3D lower_left_corner = origin - horizontal/2 - vertical/2 - Vec3D(0.0f, 0.0f, focal);

    int t = 16;
    dim3 blocks(nx/t+1, ny/t+1);
    dim3 threads(t,t);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // Lancement kernel AVEC la scène
    cudaEventRecord(start);
    render<<<blocks, threads>>>(frameBuffer, nx, ny, scene, lower_left_corner, horizontal, vertical, origin);
    cudaEventRecord(stop);
    cudaError_t launchErr = cudaGetLastError();

    if (launchErr != cudaSuccess) {
        std::cerr << "Erreur au lancement du Kernel : " << cudaGetErrorString(launchErr) << std::endl;
    }
    
    cudaError_t syncErr = cudaDeviceSynchronize();
    if (syncErr != cudaSuccess) {
        std::cerr << "Erreur pendant l'exécution du Kernel : " << cudaGetErrorString(syncErr) << std::endl;
    }
    
    cudaEventSynchronize(stop);
    cudaEventElapsedTime(&ms, start, stop);
    std::cout << "Temps GPU reel : " << ms << " ms" << std::endl;

    // --- SAUVEGARDE PPM ---
    std::ofstream ofs("Output.ppm");
    ofs << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j = ny-1; j >= 0; j--) {
        for (int i = 0; i < nx; i++) {
            size_t pixel_index = j * nx + i;
            // On clamp pour être sûr de ne pas déborder
            int ir = std::min(255, std::max(0, int(255.99 * frameBuffer[pixel_index].x)));
            int ig = std::min(255, std::max(0, int(255.99 * frameBuffer[pixel_index].y)));
            int ib = std::min(255, std::max(0, int(255.99 * frameBuffer[pixel_index].z)));
            ofs << ir << " " << ig << " " << ib << "\n";
        }
    }

    cudaFree(frameBuffer);
    cudaFree(scene->objects);
    cudaFree(scene->lights);
    cudaFree(scene);
    return 0;
}