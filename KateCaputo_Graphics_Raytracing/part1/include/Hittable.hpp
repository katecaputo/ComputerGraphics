#ifndef HITTABLE_H
#define HITTABLE_H

#include "vec3.hpp"
#include "Ray.hpp"
#include "Material.hpp"

#include <cmath>

class Hittable {
    public:
        // is the object hit by the given ray?
        virtual bool hit(const Ray&r, double t_min, double t_max,
            point3& recP, vec3& recNorm, double& recT,
            std::shared_ptr<Material>& recMtl, double& recU, double& recV) const = 0;

        // destructor for a hittable object
        virtual ~Hittable() {
            
        }
};

#endif