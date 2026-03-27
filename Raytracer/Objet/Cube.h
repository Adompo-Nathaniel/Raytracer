#ifndef CUBE_H
#define CUBE_H

#include "Objet3D.h"
#include "Material.h"

class Cube : public Objet3D {
public:
    Cube(const Vec3D& c, float width, float height, float depth, const Material& m);
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

#endif //CUBE_H