#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <memory>
#include "Objet3D.h"
#include "PointLight.h"
#include "Vec3D.h"
#include "Rayon3D.h"
#include "Intersection3D.h"

class Scene {
public:
    std::vector<std::unique_ptr<Objet3D>> objects;
    std::vector<PointLight> lights;

    void add(std::unique_ptr<Objet3D> obj);
    void addLight(const PointLight& light);

    // Trouve l'intersection la plus proche 
    bool hit(const Rayon3D& r, float t_min, float t_max, Intersection3D& rec) const;

    // Calcul de la couleur d'un rayon (Récursif)
    Vec3D castRay(const Rayon3D& ray, int depth) const;
    bool ShadowRay(const Vec3D& point, const PointLight& light,const Vec3D& normal)const;
};

#endif