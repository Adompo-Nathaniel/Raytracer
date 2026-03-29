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
#define SAMPLES_PER_PIXEL 18

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
        pixel_color = pixel_color + scene->castRayIterative(ray, 50); 
    }
    Vec3D color = pixel_color / (float)SAMPLES_PER_PIXEL;
    color.x = powf(fmax(color.x,0.0f),1.0f/2.2f);
    color.y = powf(fmax(color.y,0.0f),1.0f/2.2f);
    color.z = powf(fmax(color.z,0.0f),1.0f/2.2f);

    frameBuffer[j * max_x + i] = color;
}

/*
Reste à faire :

- Vérifier code et sortie GPU

- Test unitaires GPU & CPU (à vérifier pour CPU)

- Benchmarks pour GPU & CPU

- Rapport (2h)
*/
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

    // Scène : un rectangle en verre sur une table avec objets de chaque côté
    addObjet({ CUBE, Vec3D(0.0f, -0.5f, -5.0f), Vec3D(40.0f, 0.02f, 40.0f), 0, MatLib::Floor() });
    addObjet({ CUBE, Vec3D(0.0f, 0.05f, -5.0f), Vec3D(2.8f, 0.10f, 2.8f), 0, MatLib::TableTop() });

    // Grand rectangle en verre au centre (long et fin)
    addObjet({ CUBE, Vec3D(0.0f, 0.25f, -5.0f), Vec3D(2.5f, 0.50f, 0.15f), 0, MatLib::BlueGlass() });

    // Objets à GAUCHE du rectangle en verre
    // Sphères en or et chrome
    addObjet({ SPHERE, Vec3D(-0.8f, 0.22f, -4.2f), Vec3D(0,0,0), 0.18f, MatLib::Gold() });
    addObjet({ SPHERE, Vec3D(-1.0f, 0.22f, -4.8f), Vec3D(0,0,0), 0.16f, MatLib::Chrome() });
    addObjet({ SPHERE, Vec3D(-0.6f, 0.22f, -5.5f), Vec3D(0,0,0), 0.17f, MatLib::Red() });
    
    // Cubes en steel, blue, gold
    addObjet({ CUBE, Vec3D(-1.0f, 0.18f, -4.4f), Vec3D(0.20f, 0.20f, 0.20f), 0, MatLib::Steel() });
    addObjet({ CUBE, Vec3D(-0.7f, 0.18f, -5.2f), Vec3D(0.18f, 0.18f, 0.18f), 0, MatLib::Blue() });

    // Objets à DROITE du rectangle en verre
    // Sphères variées
    addObjet({ SPHERE, Vec3D(0.8f, 0.22f, -4.2f), Vec3D(0,0,0), 0.18f, MatLib::Green() });
    addObjet({ SPHERE, Vec3D(1.0f, 0.22f, -4.8f), Vec3D(0,0,0), 0.16f, MatLib::White() });
    addObjet({ SPHERE, Vec3D(0.6f, 0.22f, -5.5f), Vec3D(0,0,0), 0.17f, MatLib::Gold() });
    
    // Cubes variés
    addObjet({ CUBE, Vec3D(1.0f, 0.18f, -4.4f), Vec3D(0.20f, 0.20f, 0.20f), 0, MatLib::Chrome() });
    addObjet({ CUBE, Vec3D(0.7f, 0.18f, -5.2f), Vec3D(0.18f, 0.18f, 0.18f), 0, MatLib::Black() });



    addLight(PointLight(Vec3D( 1.5f, 3.0f, -2.5f), Vec3D(1.0f, 0.88f, 0.65f), 1.0f));
    addLight(PointLight(Vec3D(-2.0f, 2.5f, -4.5f), Vec3D(0.55f, 0.65f, 1.0f),  0.8f));
    //addLight(PointLight(Vec3D( 0.0f, 4.5f, -5.5f), Vec3D(1.0f, 1.0f,  1.0f),  0.5f));

    // vue plongeant de la scène
    Vec3D lookfrom(0.0f, 3.2f, -3.0f);
    Vec3D lookat(0.0f, 0.12f, -5.0f);
    Vec3D vup(0.0f, 1.0f, 0.0f);
    float vfov = 55.0f;

    // déclaration du ratio manquante
    float aspect_ratio = (float)nx / (float)ny;

    Vec3D w = (lookfrom - lookat).normalized();
    
    // calcul manuel du produit vectoriel au cas où cross() pose problème
    Vec3D u_tmp(vup.y * w.z - vup.z * w.y, 
                vup.z * w.x - vup.x * w.z, 
                vup.x * w.y - vup.y * w.x);
    Vec3D u = u_tmp.normalized();

    // second produit vectoriel manuel
    Vec3D v_cam(w.y * u.z - w.z * u.y, 
                w.z * u.x - w.x * u.z, 
                w.x * u.y - w.y * u.x);

    float theta = vfov * 3.14159265f / 180.0f;
    float h = tanf(theta / 2.0f);
    float viewport_height = 2.0f * h;
    float viewport_width = aspect_ratio * viewport_height;
    
    Vec3D horizontal_vec = u * viewport_width;
    Vec3D vertical_vec = v_cam * viewport_height;
    
    Vec3D lower_left_corner = lookfrom - horizontal_vec/2.0f - vertical_vec/2.0f - w;

    int t = 16;
    dim3 blocks(nx/t+1, ny/t+1);
    dim3 threads(t,t);

    cudaEvent_t start, stop;
    cudaEventCreate(&start);
    cudaEventCreate(&stop);

    // lancement kernel
    cudaEventRecord(start);
    render<<<blocks, threads>>>(frameBuffer, nx, ny, scene, lower_left_corner, horizontal_vec, vertical_vec, lookfrom);
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