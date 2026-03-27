#ifndef INTERSECTION3D_H
#define INTERSECTION3D_H

#include "Vec3D.h"
#include "Rayon3D.h"
#include "Material.h"

struct Intersection3D {
    Vec3D point;
    Vec3D normal;
    float t{0.0};
    bool occurred{false}; 
    bool front_face{false}; 
    Material material;

    inline void set_face_normal(const Rayon3D& r, const Vec3D& outward_normal) {
        front_face = (dot(r.direction, outward_normal) < 0.0);
        normal = front_face ? outward_normal : outward_normal * -1.0;
    }
};

#endif // INTERSECTION3D_H
