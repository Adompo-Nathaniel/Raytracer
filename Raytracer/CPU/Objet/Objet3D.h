#ifndef OBJET3D_H
#define OBJET3D_H

#include "Vec3D.h"
#include "Rayon3D.h"
#include "Intersection3D.h"

class Objet3D {
public:
    virtual ~Objet3D() = default; // Indispensable pour la mémoire
    virtual bool hit(const Rayon3D& r, float t_min, float t_max, Intersection3D& rec) const = 0;
};

#endif // OBJET3D_H