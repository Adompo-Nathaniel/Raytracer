#include <iostream>
#include "Objet/Vec3D.h"
#include "Objet/Objet3D.h"
#include "Objet/MaterialLibrary.h"

__global__ void run_gpu_tests(int* results) {
    int idx = 0;
    
    // Test 1: Précision Vec3D
    Vec3D v(1, 2, 3);
    results[idx++] = (v.length() > 3.74f && v.length() < 3.75f); 

    // Test 2: Intersection Device
    // On crée un objet "à la volée" sur le GPU
    Objet3D s;
    s.type = SPHERE;
    s.center = Vec3D(0,0,5);
    s.radius = 1.0f;
    
    Rayon3D r(Vec3D(0,0,0), Vec3D(0,0,1));
    Intersection3D inter;
    results[idx++] = s.hit(r, 0.001f, 100.0f, inter);
    results[idx++] = (inter.t > 3.9f && inter.t < 4.1f);
}

int main() {
    const int NUM_TESTS = 3;
    int h_results[NUM_TESTS];
    int *d_results;
    cudaMalloc(&d_results, NUM_TESTS * sizeof(int));

    run_gpu_tests<<<1, 1>>>(d_results);
    cudaMemcpy(h_results, d_results, NUM_TESTS * sizeof(int), cudaMemcpyDeviceToHost);

    std::cout << "=== RAPPORT DE VALIDATION GPU (CUDA) ===\n";
    const char* names[] = {"Vec3D Math Precision", "Sphere Hit Logic", "T-Distance accuracy"};
    for(int i=0; i<NUM_TESTS; i++) {
        std::cout << (h_results[i] ? "[PASS] " : "[FAIL] ") << names[i] << "\n";
    }

    cudaFree(d_results);
    return 0;
}