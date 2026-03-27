#include "Scene.h"
#include <algorithm> 
#include <cmath>     


bool Scene::hit(const Rayon3D& r, float t_min, float t_max, Intersection3D& rec) const {
    Intersection3D temp_rec;
    bool hit = false;
    float closest = t_max;

    // Sphères
    for(const auto& sphere : spheres){
        if(sphere.hit(r,t_min,closest,temp_rec)){
            hit = true;
            closest = temp_rec.t;
            rec = temp_rec;
        }
    }

    // Cubes
    for(const auto& cube : cubes){
        if(cube.hit(r,t_min,closest,temp_rec)){
            hit = true;
            closest = temp_rec.t;
            rec = temp_rec;
        }
    }
    return hit;
}

Vec3D Scene::castRay(const Rayon3D& ray, int depth) const {
    if (depth <= 0) return Vec3D(0.0f, 0.0f, 0.0f); 

    Intersection3D rec;
    if (!this->hit(ray, 0.001f, 1000.0f, rec)) {
        float t = 0.5f * (ray.direction.y + 1.0f);
        return Vec3D(0.0f, 0.0f, 0.0f) * (1.0f - t) + Vec3D(0.0f, 0.0f, 0.0f) * t;
    }

    // 1. COULEUR DE SURFACE (Phong)
    Vec3D surface_color(0.0f, 0.0f, 0.0f);
    
    // On calcule l'éclairage direct seulement si l'objet n'est pas 100% transparent
    if (rec.material.transparency < 1.0f) {
        surface_color = rec.material.ka * rec.material.color; 

        for (const auto& light : lights) {
            if(!ShadowRay(rec.point, light, rec.normal)) {
                Vec3D Light = (light.Position() - rec.point).normalized();
                Vec3D Normal = rec.normal;
                Vec3D Vector = (ray.origine - rec.point).normalized();
                Vec3D H = (Light + Vector).normalized();

                float diff = std::max(dot(Normal, Light), 0.0f);
                Vec3D diffuse = rec.material.kd * diff * rec.material.color;

                float spec = std::pow(std::max(0.0f, dot(Normal, H)), rec.material.shininess);
                Vec3D specular = rec.material.ks * spec * light.Color();

                surface_color = surface_color + (diffuse + specular) * light.Intensity();
            }
        }
    }

    // 2. RÉFLEXION ET RÉFRACTION
    Vec3D final_color = surface_color;

    // Si transparence
    if (rec.material.transparency > 0.0f) {
        float eta;
        Vec3D outward_normal;
        float cos_theta = dot(ray.direction, rec.normal);

        // On sort de l'objet
        if (cos_theta > 0.0f) {
            outward_normal = rec.normal * -1.0f;
            eta = rec.material.refractive_index;
        } 
        // On entre dans l'objet
        else {
            outward_normal = rec.normal;
            eta = 1.0f / rec.material.refractive_index;
        }

        float ref_prob = Material::reflectance(std::abs(cos_theta), eta);
        
        // Rayon Réfléchi (Miroir)
        Vec3D reflect_dir = ray.direction - rec.normal * 2.0f * dot(ray.direction, rec.normal);
        Vec3D reflection_color = castRay(Rayon3D(rec.point + rec.normal * 0.001f, reflect_dir), depth - 1);
        
        // Rayon Réfracté (Transparence)
        Vec3D refraction_color(0.0f, 0.0f, 0.0f);
        
        // Vérification de la réflexion totale interne (TIR)
        float sin_theta = std::sqrt(std::max(0.0f, 1.0f - cos_theta * cos_theta));
        if (eta * sin_theta <= 1.0f) {
            Vec3D refract_dir = Material::refract(ray.direction, outward_normal, eta);
            refraction_color = castRay(Rayon3D(rec.point + outward_normal * -0.001f, refract_dir), depth - 1);
        } else {
            // Si la lumière ne peut pas sortir, elle est entièrement reflétée
            ref_prob = 1.0f;
        }

        // Reflet + Transparence
        Vec3D dielectric_color = reflection_color * ref_prob + refraction_color * (1.0f - ref_prob);
        
        // Mélange final
        final_color = surface_color * (1.0f - rec.material.transparency) + dielectric_color * rec.material.transparency;
    }
    // Réflexion sans transparence
    else if (rec.material.reflectivity > 0.0f) {
        Vec3D reflect_dir = ray.direction - rec.normal * 2.0f * dot(ray.direction, rec.normal);
        Vec3D reflection_color = castRay(Rayon3D(rec.point + rec.normal * 0.001f, reflect_dir), depth - 1);
        
        final_color = surface_color * (1.0f - rec.material.reflectivity) + reflection_color * rec.material.reflectivity;
    }

    return final_color;
}

bool Scene::ShadowRay(const Vec3D& point, const PointLight& light, const Vec3D& normal) const {
    Vec3D to_light = light.Position() - point;
    float distance_to_light = to_light.length();
    Vec3D direction_to_light = to_light.normalized();

    // On crée un rayon du point vers la lumière avec un décalage sur la normale (pour l'acné d'ombre)
    Rayon3D shadow_ray(point + normal * 0.001f, direction_to_light); 

    Intersection3D temp_rec;
    // Si on touche un objet avant d'atteindre la lumière, alors le point est dans l'ombre
    if (this->hit(shadow_ray, 0.001f, distance_to_light, temp_rec)) {
        return true;
    }
    return false;
}