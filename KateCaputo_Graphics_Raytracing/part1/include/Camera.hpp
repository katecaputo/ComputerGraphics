#ifndef CAMERA_H
#define CAMERA_H

#include "vec3.hpp"
#include "Ray.hpp"

#include <cmath>

class Camera {
    public:
        // constructor, takes in cam poition, where it's looking, which dir is up,
        //      the vertical field of view, and image aspect ratio
        Camera(
            point3 eye,
            point3 lookAt,
            point3 up_vector,
            double vert_fov,
            double aspect,
            double ap,
            double focus_dist);

        // returns the appropriate ray given 2 vals
        Ray get_ray(double u, double v);

    private:
        // cam origin
        point3 origin;
        // lower left corner of screen
        point3 lower_left;
        // vertical and horizonal vectors of cam axis
        vec3 horizontal;
        vec3 vertical;

        // 3 cam vectors
        vec3 vecA;
        vec3 vecB;
        vec3 vecC;

        // cam lens radius
        double lens_radius;
};

#endif