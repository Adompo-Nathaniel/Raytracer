#include <iostream>
#include <vector>
#include <fstream>
#include <memory>
#include <chrono>
#include <algorithm>
#include <random>

// Fonction utilitaires
#include "Objet/Rayon3D.h"
#include "Objet/Vec3D.h"
#include "Objet/Rayon3D.h"
#include "Objet/Intersection3D.h"
#include "Objet/Objet3D.h"

// Objets spécifiques
#include "Objet/Scene.h"
#include "Objet/PointLight.h"
#include "Objet/Material.h"

#include "Objet/Sphere.h"
#include "Objet/Parallelepipede.h"

int main() {
    std::cerr << "Demarrage du Raytracer..." << std::endl;
    // Dimensions
    const int width = 1920;
    const int height = 1080; 
    std::vector<unsigned char> framebuffer(width * height * 3, 0);

    //Générateur de nombres aléatoires mt19937 plus performant que rand()
    std::mt19937 gen(1337); 
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    try {
        framebuffer.resize(width * height * 3, 0);
    } catch (const std::exception& e) {
        std::cout << "Erreur allocation memoire : " << e.what() << std::endl;
        return 1;
    }

    // Scène 3D et Objets
    Scene scene;
    scene.addLight(PointLight(Vec3D(3.0f, 1.0f, 1.0f), Vec3D(1.0f, 1.0f, 1.0f), 0.5f));

    // Matériaux 
    Material mat_red(Vec3D(255.0f, 0.0f, 0.0f),
                     Vec3D(0.1f, 0.1f, 0.1f),
                     Vec3D(0.9f, 0.9f, 0.9f), 
                     Vec3D(0.5f, 0.5f, 0.5f), 64.0f, 0.0f);

    Material mat_white(Vec3D(255.0f, 255.0f, 255.0f),
                     Vec3D(0.1f, 0.1f, 0.1f),
                     Vec3D(0.9f, 0.9f, 0.9f), 
                     Vec3D(0.5f, 0.5f, 0.5f), 64.0f, 0.0f);

    Material mat_blue(Vec3D(0.0f, 0.0f, 255.0f),
                      Vec3D(0.1f, 0.1f, 0.1f), 
                      Vec3D(0.7f, 0.7f, 0.7f), 
                      Vec3D(0.3f, 0.3f, 0.3f), 32.0f, 0.0f);

    Material chrome(Vec3D(255.0f, 255.0f, 255.0f),
                    Vec3D(0.05f, 0.05f, 0.05f),
                    Vec3D(0.9f, 0.9f, 0.9f),
                    Vec3D(0.9f, 0.9f, 0.9f), 128.0f, 1.0f);

    Material mat_floor(Vec3D(100.0f, 100.0f, 100.0f),
                       Vec3D(0.1f, 0.1f, 0.1f),
                       Vec3D(0.8f, 0.8f, 0.8f),
                       Vec3D(0.2f, 0.2f, 0.2f), 32.0f, 0.5f);
    Material star_mat(Vec3D(255.0f, 255.0f, 255.0f), 
                      Vec3D(1.0f, 1.0f, 1.0f),
                      Vec3D(0.7f, 0.7f, 0.7f), 
                      Vec3D(0.3f, 0.3f, 0.3f), 32.0f, 0.0f);

    for(int i = 0; i < 100; ++i) {
        // On génère des positions aléatoires dans le ciel
        float rx = -30.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/60.0f));
        float ry = -10.0f + static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/40.0f));
        float rz = -5.0f - static_cast<float>(rand()) / (static_cast<float>(RAND_MAX/45.0f));
        
        // On ajoute une mini sphère (rayon 0.05)
        scene.add(std::make_unique<Sphere>(Vec3D(rx, ry, rz), 0.05f, star_mat));
    }
    // Objets
    scene.add(std::make_unique<Sphere>(Vec3D(-0.7f, 0.0f, -1.5f), 0.25f, mat_red));
    scene.add(std::make_unique<Sphere>(Vec3D(0.0f, 0.0f, -1.5f), 0.25f, chrome));
    scene.add(std::make_unique<Parallelepipede>(Vec3D(0.7f, 0.0f, -1.5f), 0.4f, 0.4f, 0.4f, mat_blue));
    scene.add(std::make_unique<Parallelepipede>(Vec3D(0.0f, -0.25f, -1.5f), 10.0f, 0.01f, 10.0f, mat_white));

    // Caméra
    std::cout << "Debut du rendu..." << std::endl;
    Vec3D camera_pos(0.0f,0.0f,0.0f);

    // Configuration du Viewport
    float aspect_ratio = (float)width / height; 
    float viewport_height = 2.0f;
    float viewport_width = aspect_ratio * viewport_height; 
    Vec3D horizontal(viewport_width, 0.0f, 0.0f);
    Vec3D vertical(0.0f, viewport_height, 0.0f);
    Vec3D lower_left_corner = camera_pos - horizontal/2 - vertical/2 - Vec3D(0.0f,0.0f,1.0f);

    // Chronomètre
    auto start_time = std::chrono::high_resolution_clock::now();
    int samples_per_pixel = 32; // Anti-aliasing
    // Boucle de rendu
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            Vec3D pixel_color(0.0f, 0.0f, 0.0f);
            // Logique alliasing 
            for(int i = 0;i<samples_per_pixel;i++){
                // Préparation du Rayon
                float u = (float(x) + dis(gen)) / (width - 1);
                float v = 1.0f -(float(y) + dis(gen)) / (height - 1);

                Vec3D direction = lower_left_corner + horizontal*u + vertical*v - camera_pos;
                Rayon3D ray(camera_pos, direction);
                pixel_color = pixel_color + scene.castRay(ray, 10);
            }

            pixel_color.x = pixel_color.x / samples_per_pixel;
            pixel_color.y = pixel_color.y / samples_per_pixel;
            pixel_color.z = pixel_color.z / samples_per_pixel;

            // On écrit dans le framebuffer en s'assurant de ne pas déborder (0-255)
            //
            int pixel_index = (y * width + x) * 3;
            framebuffer[pixel_index]     = static_cast<unsigned char>(std::min(255.0f, std::max(0.0f, pixel_color.x)));
            framebuffer[pixel_index + 1] = static_cast<unsigned char>(std::min(255.0f, std::max(0.0f, pixel_color.y)));
            framebuffer[pixel_index + 2] = static_cast<unsigned char>(std::min(255.0f, std::max(0.0f, pixel_color.z)));
        }
    }

    auto end_time = std::chrono::high_resolution_clock::now();
    std::chrono::duration<float> elapsed = end_time - start_time;

    // 2. AFFICHAGE FINAL
    std::cout << "Temps : " << elapsed.count() << " secondes" << std::endl;
    std::cout << "Ecriture de Output.ppm..." << std::endl;

    std::ofstream ofs("Output.ppm", std::ios::binary);
    ofs << "P6\n" << width << " " << height << "\n255\n";
    ofs.write(reinterpret_cast<const char*>(framebuffer.data()), framebuffer.size());
    ofs.close();
    return 0;
}