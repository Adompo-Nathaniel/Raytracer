#ifndef OBJET3D_H
#define OBJET3D_H

#include "Vec3D.h"
#include "Rayon3D.h"
#include "Intersection3D.h"

enum ShapeTYPE {SPHERE, CUBE};
struct Objet3D{
    ShapeTYPE type;
    Vec3D center;
    Vec3D size;
    float radius;
    Material material;

    //__device__
__host__ __device__ bool hit(const Rayon3D& r, float t_min, float t_max, Intersection3D& inter) const {
        switch (type) {
            case SPHERE: {
                Vec3D oc = r.origine - center;
                float a = dot(r.direction, r.direction);
                float half_b = dot(oc, r.direction);
                float c = dot(oc, oc) - radius * radius;
                float discriminant = half_b * half_b - a * c;

                if (discriminant < 0.0f) return false;
                float sqrtd = sqrtf(discriminant); // fminf, fmaxf, sqrtf pour le GPU

                float root = (-half_b - sqrtd) / a;
                if (root < t_min || root > t_max) {
                    root = (-half_b + sqrtd) / a;
                    if (root < t_min || root > t_max) return false;
                }

                inter.t = root;
                inter.point = r.point_at_t(inter.t);
                Vec3D outward_normal = (inter.point - center) / radius;
                inter.set_face_normal(r, outward_normal);
                inter.material = material;
                inter.occurred = true;
                return true;
            }
            case CUBE: {
                float half_w = size.x / 2.0f;
                float half_h = size.y / 2.0f;
                float half_d = size.z / 2.0f;
                Vec3D min_p = center - Vec3D(half_w, half_h, half_d);
                Vec3D max_p = center + Vec3D(half_w, half_h, half_d);

                Vec3D inv_dir(1.0f / r.direction.x, 1.0f / r.direction.y, 1.0f / r.direction.z);

                float t1 = (min_p.x - r.origine.x) * inv_dir.x;
                float t2 = (max_p.x - r.origine.x) * inv_dir.x;
                float tmin = fminf(t1, t2);
                float tmax = fmaxf(t1, t2);

                t1 = (min_p.y - r.origine.y) * inv_dir.y;
                t2 = (max_p.y - r.origine.y) * inv_dir.y;
                tmin = fmaxf(tmin, fminf(t1, t2));
                tmax = fminf(tmax, fmaxf(t1, t2));

                t1 = (min_p.z - r.origine.z) * inv_dir.z;
                t2 = (max_p.z - r.origine.z) * inv_dir.z;
                tmin = fmaxf(tmin, fminf(t1, t2));
                tmax = fminf(tmax, fmaxf(t1, t2));

                if (tmax < tmin || tmax < 0) return false;
                
                float t_final = (tmin < 0) ? tmax : tmin;
                if (t_final < t_min || t_final > t_max) return false;

                inter.t = t_final;
                inter.point = r.point_at_t(inter.t);
                
                Vec3D d = inter.point - center;
                Vec3D normal(0,0,0);

                if (fabsf(d.x) >= half_w * 0.999f) 
                    normal = Vec3D(d.x > 0 ? 1.0f : -1.0f, 0, 0);
                else if (fabsf(d.y) >= half_h * 0.999f) 
                    normal = Vec3D(0, d.y > 0 ? 1.0f : -1.0f, 0);
                else 
                    normal = Vec3D(0, 0, d.z > 0 ? 1.0f : -1.0f);

                inter.set_face_normal(r, normal);
                inter.material = material;
                inter.occurred = true;

                return true;
            }
        }
        return false;
    }
};

#endif // OBJET3D_H