#include <iostream>
#include <fstream>
#include <iomanip>
#include <vector>
#include "Objet/Vec3D.h"
#include "Objet/Rayon3D.h"
#include "Objet/Sphere.h"
#include "Objet/Cube.h"
#include "Objet/MaterialLibrary.h"

// Helper pour le rapport
void log_test(std::ofstream& f, const std::string& name, bool success, const std::string& details = "") {
    f << std::left << std::setw(30) << name << " | " 
      << (success ? "[OK]" : "[ERREUR]") << " " << details << "\n";
}

int main() {
    std::ofstream report("rapport_tests_cpu.txt");
    report << "===========================================\n";
    report << "   RAPPORT DE TEST UNITAIRE COMPLET CPU    \n";
    report << "===========================================\n\n";

    // --- TEST MATHS VEC3D ---
    Vec3D v1(1, 2, 3), v2(0, 1, 0);
    log_test(report, "Vec3D Normalization", v2.normalized().length() == 1.0f);
    log_test(report, "Vec3D Dot Product", dot(v1, v2) == 2.0f);
    
    // --- TEST RÉFRACTION (Loi de Snell) ---
    // Un rayon arrivant à 45° dans du verre
    Vec3D incident(1, -1, 0);
    Vec3D normale(0, 1, 0);
    Vec3D refr = Material::refract(incident.normalized(), normale, 1.0f/1.5f);
    log_test(report, "Physique : Réfraction", refr.y < 0 && refr.x > 0, "Direction cohérente");

    // --- TEST INTERSECTION SPHÈRE ---
    Sphere s(Vec3D(0, 0, -5), 1.0f, MatLib::Red());
    Intersection3D inter;
    
    // Cas 1 : Face
    Rayon3D r1(Vec3D(0, 0, 0), Vec3D(0, 0, -1));
    log_test(report, "Sphere : Hit frontal", s.hit(r1, 0.001f, 100.0f, inter) && inter.t > 0);
    
    // Cas 2 : Intérieur (très important pour les objets transparents)
    Rayon3D r2(Vec3D(0, 0, -5), Vec3D(0, 0, 1));
    log_test(report, "Sphere : Hit intérieur", s.hit(r2, 0.001f, 100.0f, inter), "Détection depuis le centre");

    // --- TEST INTERSECTION CUBE ---
    Cube c(Vec3D(0,0,-10), 2, 2, 2, MatLib::Chrome());
    log_test(report, "Cube : Hit face avant", c.hit(Rayon3D(Vec3D(0,0,0), Vec3D(0,0,-1)), 0.001f, 100.0f, inter));
    log_test(report, "Cube : Miss (côté)", !c.hit(Rayon3D(Vec3D(5,0,0), Vec3D(0,0,-1)), 0.001f, 100.0f, inter));

    std::cout << "Tests complets CPU termines. Consultez rapport_tests_cpu.txt" << std::endl;
    return 0;
}