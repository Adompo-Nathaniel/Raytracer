#ifndef PARALLELEPIPED_H
#define PARALLELEPIPED_H

#include "Objet3D.h"
#include "Material.h"

class Parallelepipede : public Objet3D {
public:
    Parallelepipede(const Vec3D& c, float width, float height, float depth, const Material& m);
    virtual bool hit(const Rayon3D& r, float t_min, float t_max, Intersection3D& rec) const override;

    Vec3D getCenter() const { return center; }
    float getWidth() const { return w; }
    float getHeight() const { return h; }
    float getDepth() const { return d; }
    const Material& getMaterial() const { return material; }
    
private :
    Vec3D center;
    float w,h,d;
    Material material;
};

#endif //PARALLELEPIPED_H