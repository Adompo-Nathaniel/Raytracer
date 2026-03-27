#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include "Objet3D.h"
#include "PointLight.h"
#include "Vec3D.h"
#include "Rayon3D.h"
#include "Intersection3D.h"
#include "Sphere.h"
#include "Cube.h"

class Scene {
public:
    std::vector<Sphere> spheres;
    std::vector<Cube> cubes;
    std::vector<PointLight> lights;
    // Ajout
    void addSphere(const Sphere& sphere){spheres.push_back(sphere); }
    void addCube(const Cube& cube){cubes.push_back(cube); }
    void addLight(const PointLight& light){lights.push_back(light); }

    // Calcul de la couleur d'un rayon (Version récursif)
    Vec3D castRay(const Rayon3D& ray, int depth) const;

    // Pour trouver l'intersection la plus proche 
    bool hit(const Rayon3D& r, float t_min, float t_max, Intersection3D& rec) const;
    bool ShadowRay(const Vec3D& point, const PointLight& light,const Vec3D& normal)const;
};

#endif