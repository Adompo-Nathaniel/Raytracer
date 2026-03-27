#ifndef MATERIAL_H
#define MATERIAL_H

#include "Vec3D.h"
#include <cmath>
#include <algorithm>

struct Material {
    Vec3D color;        
    Vec3D ka, kd, ks;
    float shininess;
    float reflectivity;
    float transparency;
    float refractive_index;

    Material()
        : color(0,0,0), ka(0.1,0.1,0.1), kd(0.5,0.5,0.5), ks(0,0,0),shininess(0), reflectivity(0), transparency(0), refractive_index(1.0) {}

    Material(Vec3D c, Vec3D a, Vec3D d, Vec3D s, float sh, float r, float t, float ri)
        : color(c), ka(a), kd(d), ks(s), shininess(sh), reflectivity(r), transparency(t), refractive_index(ri) {}
    
    static float reflectance(float cos_theta, float eta) {
        float r0 = (1.0f - eta) / (1.0f + eta);
        r0 = r0 * r0;
        return r0 + (1.0f - r0) * std::pow(1.0f - cos_theta, 5);
    }

    static Vec3D refract(const Vec3D& uv, const Vec3D& n, float eta) {
        // Correction : uv * -1.0f au lieu de -1.0f * uv
        float cos_theta = std::min(dot(uv * -1.0f, n), 1.0f);
        
        // Correction : (uv + n * cos_theta) * eta au lieu de eta * (...)
        Vec3D r_out_perp = (uv + n * cos_theta) * eta;
        
        // Correction : dot(r_out_perp, r_out_perp) au lieu de length_squared()
        Vec3D r_out_parallel = n * -std::sqrt(std::abs(1.0f - dot(r_out_perp, r_out_perp)));
        
        return r_out_perp + r_out_parallel;
    }
    
};

#endif