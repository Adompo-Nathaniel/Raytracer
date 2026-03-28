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

    __host__ __device__ Material()
        : color(0,0,0), ka(0.1,0.1,0.1), kd(0.5,0.5,0.5), ks(0,0,0),shininess(0), reflectivity(0), transparency(0), refractive_index(1.0) {}

    __host__ __device__ Material(Vec3D color, Vec3D kambiant, Vec3D kdiffus, Vec3D kspeculaire, float shininess, float reflectivity, float transparency, float refractive_index)
        : color(color), ka(kambiant), kd(kdiffus), ks(kspeculaire), shininess(shininess), reflectivity(reflectivity), transparency(transparency), refractive_index(refractive_index) {}
    
    __host__ __device__ static float reflectance(float cos_theta, float eta) {
        float r0 = (1.0f - eta) / (1.0f + eta);
        r0 = r0 * r0;
        return r0 + (1.0f - r0) * powf(1.0f - cos_theta, 5);
    }

    __host__ __device__ static Vec3D refract(const Vec3D& uv, const Vec3D& n, float eta) {
        float cos_theta = fmin(dot(uv * -1.0f, n), 1.0f);

        Vec3D r_out_perp = (uv + n * cos_theta) * eta;
        Vec3D r_out_parallel = n * -sqrtf(fabs(1.0f - dot(r_out_perp, r_out_perp)));

        return r_out_perp + r_out_parallel;
    }
    
};

#endif