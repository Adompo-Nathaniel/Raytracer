#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Vec3D.h"

class PointLight {
private:
    Vec3D position;
    Vec3D color;
    float intensity;

public:
    // Constructeur : Position, Couleur (RGB 0-1), Intensité
    PointLight(const Vec3D& pos, const Vec3D& col, float inten) 
        : position(pos), color(col), intensity(inten) {}

    Vec3D Position() const { return position; }
    Vec3D Color() const { return color; }
    float Intensity() const { return intensity; }
};

#endif // POINTLIGHT_H