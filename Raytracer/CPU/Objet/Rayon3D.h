#ifndef RAYON3D_H
#define RAYON3D_H

#include "Vec3D.h"

struct Rayon3D {
    Vec3D origine;
    Vec3D direction;

    Rayon3D() = default;
    Rayon3D(const Vec3D& o, const Vec3D& d) : origine(o), direction(d.normalized()) {}

    Vec3D point_at_t(float t) const { return origine + direction * t; }
};

#endif // RAYON3D_H
