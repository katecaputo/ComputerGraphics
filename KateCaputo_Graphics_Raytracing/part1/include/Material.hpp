#ifndef MATERIAL_H
#define MATERIAL_H

#include "Ray.hpp"
#include "Texture.hpp"

#include <cmath>

class Material {
    public:
        // function to scatter light rays according to appropriate material
        //      to be overridden
        virtual bool scatter(const Ray& r_in, vec3& rec_pt, vec3& rec_normal,
            color& attn, Ray& scattered, double& hit_U, double& hit_V) const = 0;
};

// Lamertian is a type of material
class Lambertian : public Material {
    public:
        // constructor for lambertian material, sets color to given value
        Lambertian(const color& a) : albedo(std::make_shared<SolidColor>(a)) {}
        Lambertian(std::shared_ptr<Texture> a) : albedo(a) {}

        // scatters light according to lambertian material properties
        virtual bool scatter(const Ray& r_in, vec3& hit_pt, vec3& hit_normal,
            color& attn, Ray& scattered, double& hit_U, double& hit_V) 
        const override {
            // scatter direction is the normal value of the surface the ray will
            // be hitting, plus a random unit vector
            vec3 scatter_direction = hit_normal + random_unit_vector();

            // account for degenerate scatter direction (if its too small)
            if (scatter_direction.near_zero()) {
                scatter_direction = hit_normal;
            }

            // update scattered light ray and attenuation    
            scattered = Ray(hit_pt, scatter_direction);
            attn = albedo->value(hit_U, hit_V, hit_pt);
            return true;
        }

    public:
        std::shared_ptr<Texture> albedo;
};

// a Metal is a type of material
class Metal : public Material {
    public:
        // constructor for lambertian material, sets color & fuzz to given value
        // if the given fuzz value is 1 or more, set it to 1
        //      i.e - given fuzz value can not exceed 1
        Metal(const color& a, double f) : albedo(a), fuzz(f < 1 ? f : 1) {}

        // scatters light according to metal material properties
        virtual bool scatter(const Ray& r_in, vec3& hit_pt, vec3& hit_normal,
            color& attn, Ray& scattered, double& hit_U, double& hit_V) 
        const override {
            // compute reflection vector using the incoming ray and the hit surface noraml
            vec3 reflected = reflect(unit_vector(r_in.direction()), hit_normal);

            // update scattered light ray and attenuation
            scattered = Ray(hit_pt, reflected + fuzz * random_in_unit_sphere());
            attn = albedo;
            return (dot(scattered.direction(), hit_normal) > 0);
        }
    
    public:
        color albedo;
        double fuzz;
};

class Dielectric : public Material {
    public: 
        // constructor for dielectric material, sets index of refraction
        Dielectric(double ir) : ind_refr(ir) {}

        // scatters light according to dielectric material properties
        virtual bool scatter(const Ray& r_in, vec3& hit_pt, vec3& hit_normal,
            color& attn, Ray& scattered, double& hit_U, double& hit_V)
        const override {
            // set attentuation to full 1.0 for all colors
            attn = color(1.0, 1.0, 1.0); 

            // always front facing, hit_normal passed in already gone through set front function
            double ref_ratio;
            ref_ratio = 1.0 / ind_refr;

            // calcs for refraction with reflection
            vec3 unit_dir = unit_vector(r_in.direction());
            double cos_theta = fmin(dot( - unit_dir, hit_normal), 1.0);
            double sin_theta = sqrt(1.0 - cos_theta * cos_theta);

            // if there is no refraction possible (when material has higher refr ind)
            // if sin theta' is > 1, soulution DNE, glass can't refract, must reflect
            vec3 direction;
            bool can_NOT_refract = ref_ratio * sin_theta > 1.0;
            bool schlick = reflectance(cos_theta, ref_ratio) > random_double();

            if (can_NOT_refract || schlick) {
                // reflect instead, solution does NOT exist
                direction = reflect(unit_dir, hit_normal);
            } else {
                // still can do refraction, solution exists
                direction = refractVec(unit_dir, hit_normal, ref_ratio);
            }

            scattered = Ray (hit_pt, direction);
            return true;
        }

    public:
        // variable for index of refraction
        //  1.0 for air
        //  1.3 - 1.7 for glass
        //  2.4 for diamond
        double ind_refr;

    private:
        // schlick's appx for reflectance
        static double reflectance(double cos, double refl_ind) {
            double r = (1 - refl_ind) / (1 + refl_ind);
            r = r * r;
            return r + (1 - r) * pow((1 - cos), 5);
        }
};

#endif