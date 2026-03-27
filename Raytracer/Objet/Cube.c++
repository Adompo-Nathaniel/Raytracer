#include "Cube.h"
#include <algorithm>
#include <cmath>

Cube::Cube(const Vec3D& c, float width, float height, float depth, const Material& m)
    : center(c), w(width), h(height), d(depth), material(m) {}

bool Cube::hit(const Rayon3D& r, float t_min, float t_max, Intersection3D& rec) const {
    float half_w = w / 2.0f;
    float half_h = h / 2.0f;
    float half_d = d / 2.0f;
    Vec3D min_p = center - Vec3D(half_w, half_h, half_d);
    Vec3D max_p = center + Vec3D(half_w, half_h, half_d);

    // Calcul des intersections avec les 3 tranches (Slabs)
    Vec3D inv_dir(1.0f / r.direction.x, 1.0f / r.direction.y, 1.0f / r.direction.z);

    float t1 = (min_p.x - r.origine.x) * inv_dir.x;
    float t2 = (max_p.x - r.origine.x) * inv_dir.x;
    float tmin = std::min(t1, t2);
    float tmax = std::max(t1, t2);

    t1 = (min_p.y - r.origine.y) * inv_dir.y;
    t2 = (max_p.y - r.origine.y) * inv_dir.y;
    tmin = std::max(tmin, std::min(t1, t2));
    tmax = std::min(tmax, std::max(t1, t2));

    t1 = (min_p.z - r.origine.z) * inv_dir.z;
    t2 = (max_p.z - r.origine.z) * inv_dir.z;
    tmin = std::max(tmin, std::min(t1, t2));
    tmax = std::min(tmax, std::max(t1, t2));

    // Si pas d'intersection ou derrière le rayon
    if (tmax < tmin || tmax < 0) return false;
    
    float t_final = (tmin < 0) ? tmax : tmin;
    if (t_final < t_min || t_final > t_max) return false;

    // Remplissage de l'intersection
    rec.t = t_final;
    rec.point = r.point_at_t(rec.t);
    
    // Calcul de la normale
    Vec3D d = rec.point - center;
    Vec3D normal(0,0,0);

    if (std::abs(d.x) >= (w / 2.0f) * 0.999f) 
        normal = Vec3D(d.x > 0 ? 1.0f : -1.0f, 0, 0);
    else if (std::abs(d.y) >= (h / 2.0f) * 0.999f) 
        normal = Vec3D(0, d.y > 0 ? 1.0f : -1.0f, 0);
    else 
        normal = Vec3D(0, 0, d.z > 0 ? 1.0f : -1.0f);

    rec.set_face_normal(r, normal);
    rec.material = material;
    rec.occurred = true;

    return true;
}