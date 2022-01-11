#include "Sphere.hpp"

#include <cmath>

// default constructor for sphere
Sphere::Sphere() {
    center = point3(0.0, 0.0, -1.0);
    radius = 1.0;
    col = color(0.5, 0.5, 0.5);
    mat_ptr = std::make_shared<Lambertian>(color(0.5, 0.5, 0.5));
}

// consturctor for sphere with values input
Sphere::Sphere(point3 c, double r, color sphere_col, std::shared_ptr<Material> m) {
    center = c;
    radius = r;
    col = sphere_col;
    mat_ptr = m;
}

// is the sphere hit by the given ray? 
bool Sphere::hit(const Ray&r, double t_min, double t_max,
    point3& recP, vec3& recNorm, double& recT,
    std::shared_ptr<Material>& recMtl, double& recU, double& recV) const {
    vec3 oc = r.origin() - center;

    // calculate
    float a = r.direction().length_squared();
    float half_b = dot(oc, r.direction());
    float c = oc.length_squared() - radius * radius;
    float discriminant = half_b * half_b - a * c;

    if (discriminant < 0) {
        return false;
    }

    float sqrtd = sqrt(discriminant);

    // find out what the closest root is that lies in the given range
    float root = (- half_b - sqrtd) / a;
    if (root < t_min || t_max < root) {
        root = (-half_b + sqrtd) / a;
        if (root < t_min || t_max < root) {
            return false;
        }
    }

    // update our recorded info
    recMtl = mat_ptr;
    recT = root;
    recP = r.at(recT);
    vec3 outward_normal = (recP - center) / radius;

    sphere_tex_coords(outward_normal, recU, recV);

    // set face normal
    bool front_face = dot(r.direction(), outward_normal) < 0;
    if (front_face) {
        recNorm = outward_normal;
    } else {
        recNorm = - outward_normal;
    }

    return true;
}