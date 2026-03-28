#ifndef POINTLIGHT_H
#define POINTLIGHT_H

#include "Vec3D.h"

class PointLight {
private:
    Vec3D position;
    Vec3D color;
    float intensity;

public:
    // Host-> pour que le CPU puisse créer la lumière,
    // Device-> pour que le GPU puisse lire sa position/couleur
    // Constructeur : Position, Couleur (RGB 0-1), Intensité
    __host__ __device__ PointLight(const Vec3D& pos, const Vec3D& col, float inten) 
        : position(pos), color(col), intensity(inten) {}
    __host__ __device__ PointLight() : position(0,0,0), color(0,0,0), intensity(0) {}

    __host__ __device__ Vec3D Position() const { return position; }
    __host__ __device__ Vec3D Color() const { return color; }
    __host__ __device__ float Intensity() const { return intensity; }
};

#endif // POINTLIGHT_H