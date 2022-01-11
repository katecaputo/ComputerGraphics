#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "vec3.hpp"
#include "vec2.hpp"
#include "Ray.hpp"
#include "Material.hpp"
#include "Hittable.hpp"

#include <cmath>

class Triangle : public Hittable {
    public:

        // default and input constructor for sphere
        Triangle();
        Triangle(point3 v1, point3 v2, point3 v3, 
            vec2 tex1, vec2 tex2, vec2 tex3, point3 c,
            std::shared_ptr<Material> m);

        // is the sphere hit by the given ray?
        virtual bool hit(const Ray&r, double t_min, double t_max,
            point3& recP, vec3& recNorm, double& recT,
            std::shared_ptr<Material>& recMtl, double& red_U, double& rec_V) const override;
        
        // destructor for an object
        ~Triangle() {
            
        }

    public:
        // the 3 points of the triangle
        point3 pt1;
        point3 pt2;
        point3 pt3;

        // the corresponding texture coordinate points of a triangle
        vec2 uv1;
        vec2 uv2;
        vec2 uv3;

        // the normal to the triangle
        vec3 norm;

        // the color and material of the triangle
        color col;
        std::shared_ptr<Material> mat_ptr;
};

#endif