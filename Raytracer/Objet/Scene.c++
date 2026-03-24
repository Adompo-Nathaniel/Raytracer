#include "Scene.h"
#include <algorithm> // Pour std::max
#include <cmath>     // Pour std::pow

void Scene::add(std::unique_ptr<Objet3D> obj) { 
    objects.push_back(std::move(obj)); 
}

void Scene::addLight(const PointLight& light) { 
    lights.push_back(light); 
}

bool Scene::hit(const Rayon3D& r, float t_min, float t_max, Intersection3D& rec) const {
    Intersection3D temp_rec;
    bool hit_anything = false;
    float closest_so_far = t_max;

    for (const auto& obj : objects) {
        if (obj->hit(r, t_min, closest_so_far, temp_rec)) {
            hit_anything = true;
            closest_so_far = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit_anything;
}

Vec3D Scene::castRay(const Rayon3D& ray, int depth) const {
    if (depth <= 0) return Vec3D(0, 0, 0); // Limite de rebonds pour éviter les boucles infinies

    Intersection3D rec;
    // Si on ne touche rien : Fond (Dégradé bleu ciel)
    if (!this->hit(ray, 0.001f, 1000.0f, rec)) {
        float t = 0.5f * (ray.direction.y + 1.0f);
        return Vec3D(0.0f, 0.0f, 0.0f) * (1.0f - t) + Vec3D(0.0f, 0.0f, 0.0f) * t;
    }

    // --- CALCUL PHONG (Lumière locale) ---
    Vec3D local_color = rec.material.ka * rec.material.color; // Ambiant

    for (const auto& light : lights) {
        Vec3D L = (light.Position() - rec.point).normalized();
        Vec3D N = rec.normal;
        Vec3D V = (ray.origine - rec.point).normalized();
        Vec3D H = (L + V).normalized();

        // Diffuse
        float diff = std::max(dot(N, L), 0.0f);
        Vec3D diffuse = rec.material.kd * diff * rec.material.color;

        // Spéculaire
        float spec = std::pow(std::max(0.0f, dot(N, H)), rec.material.shininess);
        Vec3D specular = rec.material.ks * spec * light.Color();

        local_color = local_color + (diffuse + specular) * light.Intensity();
    }

    // --- LOGIQUE DE RÉFLEXION ---
    if (rec.material.reflectivity > 0.0f) {
        // Formule de réflexion : R = I - 2 * dot(I, N) * N
        Vec3D reflect_dir = ray.direction - rec.normal * 2.0f * dot(ray.direction, rec.normal);
        
        // On décale un peu l'origine (0.001f) pour éviter l'auto-intersection ("Acne")
        Rayon3D reflect_ray(rec.point + rec.normal * 0.001f, reflect_dir);
        
        Vec3D reflected_color = castRay(reflect_ray, depth - 1);
        
        // Interpolation entre la couleur locale et la couleur réfléchie
        local_color = local_color * (1.0f - rec.material.reflectivity) 
                    + reflected_color * rec.material.reflectivity;
    }

    return local_color;
}