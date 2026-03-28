#ifndef INTERSECTION3D_H
#define INTERSECTION3D_H

#include "Vec3D.h"
#include "Rayon3D.h"
#include "Material.h"

struct Intersection3D {
    Vec3D point;
    Vec3D normal;
    float t{0.0f};
    bool occurred{false}; 
    bool front_face{false}; 
    Material material;

    __host__ __device__ inline void set_face_normal(const Rayon3D& r, const Vec3D& outward_normal) {
        front_face = (dot(r.direction, outward_normal) < 0.0f);
        normal = front_face ? outward_normal : outward_normal * -1.0f;
    }
};

#endif // INTERSECTION3D_H
