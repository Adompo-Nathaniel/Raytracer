#include <iostream>
#include <chrono>
#include <vector>
#include "Objet/Objet3D.h"
#include "Objet/Rayon3D.h"
#include "Objet/Intersection3D.h"

int main() {
    // On utilise le même nombre de rayons que sur GPU pour comparer
    int n = 100000000;// 100 Millions
    int total_hits = 0;

    // Simulation d'une sphère et d'un rayon
    // On les définit hors de la boucle pour mesurer uniquement le coût du calcul 'hit'
    Objet3D s;
    s.type = SPHERE; 
    s.center = Vec3D(0,0,5); 
    s.radius = 1.0f;
    Rayon3D r(Vec3D(0,0,0), Vec3D(0,0,1));
    Intersection3D inter;

    std::cout << "Lancement du Benchmark CPU (Intersection)..." << std::endl;

    auto start = std::chrono::high_resolution_clock::now();
    
    for (int i = 0; i < n; i++) {
        // Le CPU fait exactement la même opération que le thread GPU
        if(s.hit(r, 0.001f, 100.0f, inter)) {
            total_hits++;
        }
    }
    
    auto end = std::chrono::high_resolution_clock::now();
    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double seconds = diff / 1000000.0;

    std::cout << "--- BENCHMARK CPU (10M Rayons) ---" << std::endl;
    std::cout << "Temps total  : " << seconds * 1000.0 << " ms" << std::endl;
    std::cout << "Performance  : " << (n / seconds) / 1e6 << " M rayons/sec" << std::endl;
    std::cout << "Hits         : " << total_hits << std::endl;

    return 0;
}