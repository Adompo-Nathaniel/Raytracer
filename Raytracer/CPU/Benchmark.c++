#include <iostream>
#include <chrono>
#include <vector>
#include "Objet/Sphere.h"
#include "Objet/MaterialLibrary.h"

int main() {
    const int N = 100000000; // 100 Millions de rayons
    Sphere s(Vec3D(0, 0, -5), 1.0f, MatLib::Red());
    Rayon3D r(Vec3D(0,0,0), Vec3D(0,0,-1));
    Intersection3D inter;
    
    int hits = 0; // On accumule pour éviter que le compilo supprime la boucle
    
    auto start = std::chrono::high_resolution_clock::now();
    for(int i = 0; i < N; ++i) {
        if(s.hit(r, 0.001f, 100.0f, inter)) {
            hits++;
        }
    }
    auto end = std::chrono::high_resolution_clock::now();

    auto diff = std::chrono::duration_cast<std::chrono::microseconds>(end - start).count();
    double seconds = diff / 1000000.0;
    
    std::cout << "--- BENCHMARK CPU INTERSECTION ---" << std::endl;
    std::cout << "Rayons calcules : " << N << std::endl;
    std::cout << "Temps total     : " << seconds << " s" << std::endl;
    std::cout << "Vitesse         : " << (N / seconds) / 1e6 << " Millions de rayons/sec" << std::endl;
    std::cout << "Verification    : " << hits << " hits enregistres." << std::endl;
    
    return 0;
}