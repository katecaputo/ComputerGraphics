#ifndef RAY_H
#define RAY_H

#include "vec3.hpp"

#include <cmath>

class Ray {
    public:
        // default and input constructors for a Ray
        Ray();
        Ray(const point3& origin, const vec3& direction);

        // returns the point and origin of a ray
        point3 origin() const;
        vec3 direction() const;

        // IMPORTANT find the point on the ray given an input of time (or other units)
        point3 at(double t) const;

    public:
        point3 orig;
        vec3 dir;
};

#endif