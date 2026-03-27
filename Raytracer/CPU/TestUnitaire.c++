#include <iostream>
#include <fstream>
#include <chrono>
#include <vector>
#include <iomanip>

#include "Objet/Vec3D.h"
#include "Objet/Rayon3D.h"
#include "Objet/Sphere.h"
#include "Objet/Scene.h"
#include "Objet/MaterialLibrary.h"

using namespace std;
using namespace std::chrono;

// Force le CPU à ne pas ignorer le calcul (Barrière d'optimisation standard)
void doNotOptimize(float val) {
    volatile float sink = val;
    (void)sink;
}

struct PerfResult {
    string label;
    double avg_ns;
    double throughput; // ops/sec
};

void log_to_file(const vector<PerfResult>& results) {
    ofstream ofs("CPU_Caracterisation_Report.txt", ios::app);
    auto now = system_clock::to_time_t(system_clock::now());
    ofs << "--- RAPPORT DE CARACTERISATION CPU : " << ctime(&now);
    ofs << left << setw(20) << "FONCTION" << setw(20) << "MOYENNE (ns)" << "DEBIT (ops/s)" << endl;
    
    for(auto& r : results) {
        ofs << left << setw(20) << r.label 
            << setw(20) << fixed << setprecision(2) << r.avg_ns 
            << scientific << setprecision(2) << r.throughput << endl;
    }
    ofs << "------------------------------------------------------------\n\n";
    ofs.close();
}

/*int main() {
    vector<PerfResult> results;
    const long long iters = 10'000'000;

    // 1. TEST UNITAIRE : MATHS (Vec3D::dot)
    // Utile pour comparer avec les registres/unités FP32 du GPU
    {
        Vec3D a(1.2, 3.4, 5.6), b(7.8, 9.0, 1.2);
        auto s = high_resolution_clock::now();
        for(long long i=0; i < iters; ++i) {
            doNotOptimize(dot(a, b));
            a.x += 0.001f; // Evite la pré-computation
        }
        auto e = high_resolution_clock::now();
        double total_s = duration<double>(e-s).count();
        results.push_back({"Vec3D::dot", (total_s*1e9)/iters, iters/total_s});
    }

    // 2. TEST UNITAIRE : GEOMETRIE (Sphere::hit)
    // C'est ici que tu mesureras le gain des coeurs CUDA sur le calcul algébrique
    {
        Sphere s(Vec3D(0,0,-5), 1.0f, MatLib::Red());
        Rayon3D r(Vec3D(0,0,0), Vec3D(0,0,-1));
        Intersection3D rec;
        auto start = high_resolution_clock::now();
        for(long long i=0; i < iters; ++i) {
            s.hit(r, 0.001f, 1000.0f, rec);
        }
        auto end = high_resolution_clock::now();
        double total_s = duration<double>(end-start).count();
        results.push_back({"Sphere::hit", (total_s*1e9)/iters, iters/total_s});
    }

    // 3. TEST UNITAIRE : TRAVERSÉE DE SCÈNE (Scene::hit)
    // Crucial pour comparer l'efficacité du parallélisme massif vs boucle for CPU
    {
        Scene scene;
        for(int i=0; i<1000; ++i) scene.add(make_unique<Sphere>(Vec3D(i,0,-50), 0.5f, MatLib::Red()));
        Rayon3D r(Vec3D(0,0,0), Vec3D(0,0,-1));
        Intersection3D rec;
        
        const int iters_scene = 100'000;
        auto start = high_resolution_clock::now();
        for(int i=0; i < iters_scene; ++i) {
            scene.hit(r, 0.001f, 1000.0f, rec);
        }
        auto end = high_resolution_clock::now();
        double total_s = duration<double>(end-start).count();
        results.push_back({"Scene::hit (10000 obj)", (total_s*1e9)/iters_scene, iters_scene/total_s});
    }
    log_to_file(results);
    return 0;
}*/