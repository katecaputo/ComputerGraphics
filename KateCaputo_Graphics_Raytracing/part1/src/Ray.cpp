#include "Ray.hpp"

// default constructor for a Ray
Ray::Ray() {

}

// constructor for a ray with origin and direction inputs
Ray::Ray(const point3& origin, const vec3& direction): 
    orig(origin), dir(direction)
    {}

// returns the origin of the ray
point3 Ray::origin() const  {
    return orig;
}

// returns the direction of the ray
vec3 Ray::direction() const {
    return dir;
}

// returns the point on the ray that the given unit dictates
point3 Ray::at(double t) const {
    return orig + t * dir;
}