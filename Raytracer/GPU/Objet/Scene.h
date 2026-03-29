#ifndef SCENE_H
#define SCENE_H

#include "Objet3D.h"
#include "PointLight.h"

class Scene {
public:
    Objet3D* objects;
    int num_objects;
    PointLight* lights;
    int num_lights;

    __device__ bool hit(const Rayon3D& r, float t_min, float t_max, Intersection3D& rec) const {
        Intersection3D temp_rec;
        bool hit_anything = false;
        float closest = t_max;
        for(int i = 0; i < num_objects; i++) {
            if(objects[i].hit(r, t_min, closest, temp_rec)) {
                hit_anything = true;
                closest = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }

    __device__ bool ShadowRay(const Vec3D& point, const PointLight& light, const Vec3D& normal) const {
        Vec3D to_light = light.Position() - point;
        float dist = to_light.length();
        Rayon3D shadow_ray(point + normal * 0.01f, to_light.normalized()); 
        Intersection3D temp;
        return this->hit(shadow_ray, 0.001f, dist, temp);
    }

    __device__ Vec3D castRayIterative(Rayon3D current_ray, int max_depth) const {
        Vec3D accumulated_color(0.0f, 0.0f, 0.0f);
        Vec3D throughput(1.0f, 1.0f, 1.0f);

        for (int depth = 0; depth < max_depth; depth++) {
            Intersection3D rec;
            if (!this->hit(current_ray, 0.001f, 1000.0f, rec)) {
                float t = 0.5f * (current_ray.direction.y + 1.0f); // 0 = horizon, 1 = zénith
                
                // 1. Définition des couleurs extrêmes
                Vec3D horizon_color(0.72f, 0.78f, 0.88f);
                Vec3D zenith_color(0.25f, 0.35f, 0.55f);
                Vec3D sun_glow(0.0f, 0.0f, 0.0f);

                // 2. Interpolation de base du ciel
                Vec3D base_sky = zenith_color * t + horizon_color * (1.0f - t);

                // 3. Ajout de l'éclat du soleil (atténuation exponentielle très rapide vers le haut)
                float glow_intensity = expf(-12.0f * t); 

                // 4. Couleur finale
                Vec3D sky_color = base_sky + sun_glow * glow_intensity;
                
                // Clamp pour éviter de dépasser 1.0 et créer des artefacts
                sky_color.x = fminf(sky_color.x, 1.0f);
                sky_color.y = fminf(sky_color.y, 1.0f);
                sky_color.z = fminf(sky_color.z, 1.0f);

                accumulated_color = accumulated_color + throughput * sky_color;
                break;
            }

            Vec3D local_lighting(0.0f, 0.0f, 0.0f);
            Vec3D color_norm = rec.material.color / 255.0f;

            // PHONG LOCAL
            if (rec.material.transparency < 1.0f) {
                // Ambiant
                local_lighting = rec.material.ka * color_norm;

                for (int i = 0; i < num_lights; i++) {
                    if (!ShadowRay(rec.point, lights[i], rec.normal)) {
                        Vec3D L = (lights[i].Position() - rec.point).normalized();
                        Vec3D V = current_ray.direction * -1.0f;
                        Vec3D H = (L + V).normalized();

                        float n_dot_l = fmaxf(dot(rec.normal, L), 0.0f);
                        float n_dot_h = fmaxf(dot(rec.normal, H), 0.0f);

                        Vec3D diffuse = rec.material.kd * n_dot_l * color_norm;
                        Vec3D specular = rec.material.ks * powf(n_dot_h, rec.material.shininess) * lights[i].Color();

                        // CORRECTION : On ne multiplie PAS le spéculaire par (1-reflectivity)
                        // Le spéculaire est le reflet direct de la lampe, il doit rester brillant.
                        local_lighting = local_lighting + (diffuse * (1.0f - rec.material.reflectivity) + specular) * lights[i].Intensity();
                    }
                }
            }
            
            accumulated_color = accumulated_color + throughput * local_lighting;

            // CALCUL DU REBOND (RÉFLEXION / RÉFRACTION)
            if (rec.material.transparency > 0.0f) {
                float cos_theta = fminf(dot(current_ray.direction * -1.0f, rec.normal), 1.0f);
                Vec3D outward_normal = (dot(current_ray.direction, rec.normal) > 0.0f) ? rec.normal * -1.0f : rec.normal;
                float eta = (dot(current_ray.direction, rec.normal) > 0.0f) ? rec.material.refractive_index : 1.0f / rec.material.refractive_index;
                
                float reflect_prob = Material::reflectance(cos_theta, eta);
                float sin_theta = sqrtf(fmaxf(0.0f, 1.0f - cos_theta * cos_theta));

                if (eta * sin_theta <= 1.0f && reflect_prob < 0.9f) {
                    current_ray = Rayon3D(rec.point + outward_normal * -0.001f, Material::refract(current_ray.direction, outward_normal, eta));
                    throughput = throughput * rec.material.transparency * (1.0f - reflect_prob);
                } else {
                    current_ray = Rayon3D(rec.point + rec.normal * 0.001f, current_ray.direction - rec.normal * 2.0f * dot(current_ray.direction, rec.normal));
                    throughput = throughput * reflect_prob;
                }
            }
            else if (rec.material.reflectivity > 0.0f) {
                // RÉFLEXION MÉTAL : On utilise ks pour teinter le reflet (essentiel pour l'OR)
                Vec3D reflect_dir = current_ray.direction - rec.normal * 2.0f * dot(current_ray.direction, rec.normal);
                current_ray = Rayon3D(rec.point + rec.normal * 0.001f, reflect_dir);
                throughput = throughput * rec.material.reflectivity * rec.material.ks;
            }
            else break;

            if (throughput.x < 0.01f && throughput.y < 0.01f && throughput.z < 0.01f) break;
        }
        return accumulated_color;
    }
};

#endif