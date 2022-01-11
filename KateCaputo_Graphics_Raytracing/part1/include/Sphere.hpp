#ifndef SPHERE_H
#define SPHERE_H

#include "vec3.hpp"
#include "Ray.hpp"
#include "Material.hpp"
#include "Hittable.hpp"

#include <cmath>

class Sphere : public Hittable {
    public:

        // default and input constructor for sphere
        Sphere();
        Sphere(point3 c, double r, color col, std::shared_ptr<Material> m);

        // is the sphere hit by the given ray?
        virtual bool hit(const Ray&r, double t_min, double t_max,
            point3& recP, vec3& recNorm, double& recT,
            std::shared_ptr<Material>& recMtl, double& recU, double& recV) const override;

        // destructor for a sphere
        ~Sphere() {
            
        }

    public:
        // center, radius, color, and material of the sphere
        point3 center;
        double radius;
        color col;
        std::shared_ptr<Material> mat_ptr;

    private:
        // p is a point on the unit sphere, u is angle around Y axis from X = -1
        // v is angle btwn Y = +- 1
        static void sphere_tex_coords(const point3& p, double& u, double& v) {
            float pi = 3.1415926535;

            float theta = acos(- p.y());
            float phi = atan2(- p.z(), p.x()) + pi;

            u = phi = (2 * pi);
            v = theta / pi;
        }
};

#endif