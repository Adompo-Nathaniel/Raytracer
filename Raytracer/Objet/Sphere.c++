#include "Sphere.h"
#include <cmath>

Sphere::Sphere(const Vec3D& c, float r, const Material& m) 
    : center(c), radius(r), material(m) {}

bool Sphere::hit(const Rayon3D& r, float t_min, float t_max, Intersection3D& rec) const {
    Vec3D oc = r.origine - center;
    float a = dot(r.direction, r.direction);
    float half_b = dot(oc, r.direction);
    float c = dot(oc, oc) - radius * radius;
    float discriminant = half_b*half_b - a*c;

    if (discriminant < 0.0f) return false;
    float sqrtd = std::sqrt(discriminant);

    float root = (-half_b - sqrtd) / a;
    if (root < t_min || root > t_max) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || root > t_max) return false;
    }

    rec.t = root;
    rec.point = r.point_at_t(rec.t);
    Vec3D outward_normal = (rec.point - center) / radius;
    rec.set_face_normal(r, outward_normal);
    rec.material = material;
    rec.occurred = true;
    return true;
}

// Getters (Ok puisque supprimés du corps de la classe dans le .h)
Vec3D Sphere::getCenter() const { return center; }
float Sphere::getRadius() const { return radius; }
const Material& Sphere::getMaterial() const { return material; }