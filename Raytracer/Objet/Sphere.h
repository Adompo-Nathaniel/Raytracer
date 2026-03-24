// Sphere.h
#ifndef SPHERE_H
#define SPHERE_H

#include "Objet3D.h"

class Sphere : public Objet3D {
public:
    Sphere(const Vec3D& c, float r, const Material& m);

    virtual bool hit(const Rayon3D& r, float t_min, float t_max, Intersection3D& rec) const override;

    Vec3D getCenter() const;
    float getRadius() const;
    const Material& getMaterial() const;

private:
    Vec3D center;
    float radius;
    Material material;
};

#endif