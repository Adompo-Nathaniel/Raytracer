#ifndef VEC3D_H
#define VEC3D_H

#include <cmath>
#ifndef __CUDACC__
    #define __host__
    #define __device__
#endif

class Vec3D {
public:
    float x, y, z;

    
    __host__ __device__  Vec3D() : x(0.0f), y(0.0f), z(0.0f){}
    __host__ __device__ Vec3D(float x_, float y_, float z_) : x(x_), y(y_), z(z_) {}

    __host__ __device__ Vec3D operator+(const Vec3D& o) const { return Vec3D(x + o.x, y + o.y, z + o.z); }
    __host__ __device__ Vec3D operator-(const Vec3D& o) const { return Vec3D(x - o.x, y - o.y, z - o.z); }
    __host__ __device__ Vec3D operator*(const Vec3D& v) const { return Vec3D(x * v.x, y * v.y, z * v.z); }

    __host__ __device__ Vec3D operator*(float s) const { return Vec3D(x * s, y * s, z * s); }
    __host__ __device__ Vec3D operator/(float s) const { return Vec3D(x / s, y / s, z / s); }

    __host__ __device__ float length() const { return sqrtf(x*x + y*y + z*z); }
    __host__ __device__ Vec3D normalized() const {
        float len = length();
        return (len > 0) ? *this / len : Vec3D(0,0,0);
    }
};

__host__ __device__ inline float dot(const Vec3D& a, const Vec3D& b) {return a.x*b.x + a.y*b.y + a.z*b.z; }

#endif