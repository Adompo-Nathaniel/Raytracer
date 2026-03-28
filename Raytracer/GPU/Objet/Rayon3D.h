#ifndef RAYON3D_H
#define RAYON3D_H

#include "Vec3D.h"

struct Rayon3D {
    Vec3D origine;
    Vec3D direction;

    __host__ __device__ Rayon3D() : origine(0.0f,0.0f,0.0f), direction(0.0f,0.0f,0.0f){}
    __host__ __device__ Rayon3D(const Vec3D& o, const Vec3D& d) : origine(o), direction(d.normalized()) {}

    __host__ __device__ Vec3D point_at_t(float t) const { return origine + direction * t; }
};

#endif // RAYON3D_H
