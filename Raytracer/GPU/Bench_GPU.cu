#include <iostream>
#include "Objet/Objet3D.h"

__global__ void bench_intersections(int n, int* total_hits) {
    int i = blockIdx.x * blockDim.x + threadIdx.x;
    if (i < n) {
        Objet3D s;
        s.type = SPHERE; s.center = Vec3D(0,0,5); s.radius = 1.0f;
        Rayon3D r(Vec3D(0,0,0), Vec3D(0,0,1));
        Intersection3D inter;
        if(s.hit(r, 0.001f, 100.0f, inter)) {
            atomicAdd(total_hits, 1);
        }
    }
}

int main() {
    int n = 100000000; // 100 Millions
    int *d_hits, h_hits = 0;
    cudaMalloc(&d_hits, sizeof(int));
    cudaMemset(d_hits, 0, sizeof(int));

    cudaEvent_t start, stop;
    cudaEventCreate(&start); cudaEventCreate(&stop);

    cudaEventRecord(start);
    bench_intersections<<<(n+255)/256, 256>>>(n, d_hits);
    cudaEventRecord(stop);
    cudaEventSynchronize(stop);

    float ms;
    cudaEventElapsedTime(&ms, start, stop);
    cudaMemcpy(&h_hits, d_hits, sizeof(int), cudaMemcpyDeviceToHost);

    std::cout << "--- BENCHMARK GPU (10M Rayons) ---" << std::endl;
    std::cout << "Temps total  : " << ms << " ms" << std::endl;
    std::cout << "Performance  : " << (n / (ms/1000.0)) / 1e6 << " M rayons/sec" << std::endl;

    cudaFree(d_hits);
    return 0;
}