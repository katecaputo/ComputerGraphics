#include "Camera.hpp"

#include <cmath>

// helper function for converting a given angle in degrees to radians
double degrees_to_rad(double degrees) {
    const double pi = 3.1415926535897932385;
    return degrees * pi / 180.0;
}

// camera constructor
Camera::Camera(point3 eye, point3 lookAt, point3 up_vector, 
    double vert_fov, double aspect, double ap, double focus_dist) {

    // variables
    const double theta = degrees_to_rad(vert_fov);
    const double height = tan(theta / 2);
    const double viewport_height = 2.0 * height;
    const double viewport_width = aspect * viewport_height;

    // LOOK ALONG vector of cam
    vecA = unit_vector(eye - lookAt);
    // X/RIGHT VECTOR of cam
    vecB = unit_vector(cross(up_vector, vecA)); 
    // UP VECTOR of cam
    vecC = cross( vecA, vecB);

    // calculates and sets camera properties
    lens_radius = ap / 2;
    origin = eye;
    horizontal = focus_dist * viewport_width * vecB;
    vertical = focus_dist * viewport_height * vecC;
    lower_left = origin - horizontal / 2 - vertical / 2 - focus_dist * vecA;
} 

// returns a ray from the camera, given 2 values
Ray Camera::get_ray(double u, double v) {
    // lens calculations
    vec3 calc_lens_rad = lens_radius * random_in_unit_disk();
    vec3 offset = vecA * calc_lens_rad.x() + vecB * calc_lens_rad.y();
    point3 full_offset = origin + offset;
    vec3 full_dir = lower_left + u * horizontal + v * vertical - origin - offset;

    return Ray(full_offset, full_dir);
}