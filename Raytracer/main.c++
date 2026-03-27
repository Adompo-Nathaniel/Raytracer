#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <chrono>
#include <algorithm>
#include <random>
#include <iomanip>

// Fonction utilitaires
#include "Objet/Rayon3D.h"
#include "Objet/Vec3D.h"
#include "Objet/Intersection3D.h"
#include "Objet/Objet3D.h"

// Objets spécifiques
#include "Objet/Scene.h"
#include "Objet/PointLight.h"
#include "Objet/Material.h"
#include "Objet/MaterialLibrary.h"
#include "Objet/Sphere.h"
#include "Objet/Cube.h"

// Anti-aliasing
#define SAMPLES_PER_PIXEL 8 

std::chrono::nanoseconds total_hit_time(0);
std::chrono::nanoseconds total_castray_time(0);


int main() {
    std::cerr << "Demarrage du Raytracer..." << std::endl;
    
    // Dimensions
    const int width = 1920;
    const int height = 1080; 
    std::vector<unsigned char> framebuffer(width * height * 3, 0);

    // Générateur de nombres aléatoires
    std::mt19937 gen(1337); 
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    try {
        framebuffer.resize(width * height * 3, 0);
    } catch (const std::exception& e) {
        std::cout << "Erreur allocation memoire : " << e.what() << std::endl;
        return 1;
    }   

    // Scène + objet
    Scene scene;

    // Spheres
    scene.addSphere(Sphere(Vec3D(-0.7f, 0.0f, -1.5f), 0.25f, MatLib::Red()));
    scene.addSphere(Sphere(Vec3D(0.0f, 0.0f, -1.5f), 0.25f, MatLib::Chrome()));
    scene.addSphere(Sphere(Vec3D(0.7f, 0.0f, -2.5f), 0.25f, MatLib::Red()));
    for(int i = 0; i < 100; ++i) {
        // Positions aléatoires
        float rx = -30.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/60.0f));
        float ry = -10.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/40.0f));
        float rz = -5.0f - static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/45.0f));
        scene.addSphere(Sphere(Vec3D(rx, ry, rz), 0.05f, MatLib::Star_mat()));
    }

    // Cubes
    scene.addCube(Cube(Vec3D(0.7f, 0.0f, -1.5f), 0.4f, 0.4f, 0.4f, MatLib::BlueGlass()));
    scene.addCube(Cube(Vec3D(0.0f, -0.25f, -1.5f), 10.0f, 0.01f, 10.0f, MatLib::Floor()));

    // Lumière
    scene.addLight(PointLight(Vec3D(3.0f, 1.0f, 1.0f), Vec3D(1.0f, 1.0f, 1.0f), 0.5f));

    // Caméra
    Vec3D camera_pos(0.0f,0.0f,0.0f);
    
    // Viewport
    float aspect_ratio = (float)width / height; 
    float viewport_height = 2.0f;
    float viewport_width = aspect_ratio * viewport_height; 
    Vec3D horizontal(viewport_width, 0.0f, 0.0f);
    Vec3D vertical(0.0f, viewport_height, 0.0f);
    Vec3D lower_left_corner = camera_pos - horizontal/2 - vertical/2 - Vec3D(0.0f,0.0f,1.0f);

    // Chronomètre Global
    auto start_render = std::chrono::high_resolution_clock::now();

    // Boucle de rendu
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Vec3D pixel_color(0.0f, 0.0f, 0.0f);
            
            // Logique alliasing 
            for(int i = 0; i < SAMPLES_PER_PIXEL; i++){
                float u = (float(x) + dis(gen)) / (width - 1);
                float v = 1.0f -(float(y) + dis(gen)) / (height - 1);

                Vec3D direction = lower_left_corner + horizontal*u + vertical*v - camera_pos;
                Rayon3D ray(camera_pos, direction);

                // Lancer de rayon
                auto t1 = std::chrono::high_resolution_clock::now();
                pixel_color = pixel_color + scene.castRay(ray, 10);
                auto t2 = std::chrono::high_resolution_clock::now();
                total_castray_time += std::chrono::duration_cast<std::chrono::nanoseconds>(t2 - t1);
            }

            pixel_color = pixel_color / (float)SAMPLES_PER_PIXEL;

            // On écrit dans le framebuffer en s'assurant de ne pas déborder (0-255)
            int pixel_index = (y * width + x) * 3;
            framebuffer[pixel_index]     = static_cast<unsigned char>(std::min(255.0f, std::max(0.0f, pixel_color.x)));
            framebuffer[pixel_index + 1] = static_cast<unsigned char>(std::min(255.0f, std::max(0.0f, pixel_color.y)));
            framebuffer[pixel_index + 2] = static_cast<unsigned char>(std::min(255.0f, std::max(0.0f, pixel_color.z)));
        }
    }

    auto end_render = std::chrono::high_resolution_clock::now();
    
    // Bilan temps
    auto diff_ms = [](auto start, auto end) { 
        return std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count(); 
    };

    std::cout << "\n--- STATISTIQUES DE PERFORMANCE ---" << std::endl;
    std::cout << "Temps d'execution (CPU): " << diff_ms(start_render, end_render) / 1000.0f << " s" << std::endl;
    std::cout << "Temps total dans castRay() : " << total_castray_time.count() / 1e9 << " s" << std::endl;

    std::ofstream ofs("Output.ppm", std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    ofs.write(reinterpret_cast<const char*>(framebuffer.data()), framebuffer.size());
    ofs.close();

    return 0;
}