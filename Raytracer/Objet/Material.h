#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec3D.h"

struct Material {
    Vec3D color;        
    Vec3D ka;     // Coefficient Ambiant
    Vec3D kd;     // Coefficient Diffus
    Vec3D ks;     // Coefficient Spéculaire
    float shininess;
    float reflectivity;
    Material() = default;
    Material(Vec3D c, Vec3D a, Vec3D d, Vec3D s, float sh, float r)
        : color(c), ka(a), kd(d), ks(s), shininess(sh), reflectivity(r) {}
};

#endif