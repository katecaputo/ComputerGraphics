#ifndef VEC3_H
#define VEC3_H

#include <cmath>
#include <iostream>

#include "Utility.hpp"

using std::sqrt;

class vec3 {
    public:
        // vec3 default and input constructors
        vec3();
        vec3(double e0, double e1, double e2);

        // returns values of vectors
        double x() const;
        double y() const;
        double z() const;

        // vec3 operators, largely unused
        vec3 operator-() const;
        double operator[](int i) const;
        double& operator[](int i);
        vec3& operator+=(const vec3 &v);
        vec3& operator*=(const double t);
        vec3& operator/=(const double t);

        // returns length and length squared of a vector
        double length() const;
        double length_squared() const;

        // generate a random vec3
        inline static vec3 random() {
            return vec3(random_double(), random_double(), random_double());
        }

        // generate a random vec3 given max and min constraints
        inline static vec3 random(double min, double max) {
            return vec3(random_double(min, max),
                        random_double(min, max),
                        random_double(min, max));
        }

        // is the vector close to zero in all dimensions?
        bool near_zero() const;

    public:
        double e[3];
};

// vec3 alias for 3d point
using point3 = vec3;
// vec3 alias for an rgb color
using color = vec3;


// vec3 utility functions, mostly unused

// prints out contents of a vector
inline std::ostream& operator<<(std::ostream &out, const vec3 &v) {
    return out << v.e[0] << ' ' << v.e[1] << ' ' << v.e[2];
}

// adds 2 given vectors together and returns result
inline vec3 operator+(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] + v.e[0], u.e[1] + v.e[1], u.e[2] + v.e[2]);
}

// subtracts 2 given vectors and returns the result
inline vec3 operator-(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] - v.e[0], u.e[1] - v.e[1], u.e[2] - v.e[2]);
}

// multiplies 2 given vectors together and returns the result
inline vec3 operator*(const vec3 &u, const vec3 &v) {
    return vec3(u.e[0] * v.e[0], u.e[1] * v.e[1], u.e[2] * v.e[2]);
}

// multiplies a given vector by a given scalar value and returns the result
inline vec3 operator*(double t, const vec3 &v) {
    return vec3(t*v.e[0], t*v.e[1], t*v.e[2]);
}

// multiplies a given vector by a given scalar value and returns the result
//      if values are given in opposite order
inline vec3 operator*(const vec3 &v, double t) {
    return t * v;
}

// divides a given vector by a given scalar value
inline vec3 operator/(vec3 v, double t) {
    return (1/t) * v;
}

// finds the dot product of 2 given vectors
inline double dot(const vec3 &u, const vec3 &v) {
    return u.e[0] * v.e[0]
         + u.e[1] * v.e[1]
         + u.e[2] * v.e[2];
}

// find the cross product of 2 given vectors
inline vec3 cross(const vec3 &u, const vec3 &v) {
    return vec3(u.e[1] * v.e[2] - u.e[2] * v.e[1],
                u.e[2] * v.e[0] - u.e[0] * v.e[2],
                u.e[0] * v.e[1] - u.e[1] * v.e[0]);
}

// makes a given vector into a unit vector
inline vec3 unit_vector(vec3 v) {
    return v / v.length();
}

// generate a random vec3 that's inside of a unit sphere
inline vec3 random_in_unit_sphere() {
    while (true) {
        vec3 p = vec3::random(-1, 1);
        if (p.length_squared() >= 1) {
            continue;
        }
        return p;
    }
}

// genereate a random vec3 that's inside the hemishpere for uniform scatter
// not dependent on normals
inline vec3 random_in_hemisphere(const vec3& normal) {
    vec3 in_unit_sphere = random_in_unit_sphere();
    // if in same hemisphere as normal
    if (dot(in_unit_sphere, normal) > 0.0) {
        return in_unit_sphere;
    }
    else {
        return -in_unit_sphere;
    }
}

// generates a random unit vector that's inside a unit sphere
inline vec3 random_unit_vector() {
    return unit_vector(random_in_unit_sphere());
}

// generates a random vec3 in the unit circle
inline vec3 random_in_unit_disk() {
    while (true) {
        vec3 p = vec3(random_double(-1, 1), random_double(-1, 1), 0);
        if (p.length_squared() >= 1) {
            continue;
        }
        return p;
    }
}

// computes reflection for a vector - for metallic materials
inline vec3 reflect(const vec3& v, const vec3& n) {
    return v - 2 * dot(v, n) * n;
}

// computes refraction for a vector - for dielectric materials
inline vec3 refractVec(const vec3& unit_vec, const vec3& v_norm, double eta_frac) {
    // find (eta/eta') * (R + (-R dot norm) * norm) ------ perpendicular to n'
    //      given unit_vec is R
    double cos_theta = fmin(dot(- unit_vec, v_norm), 1.0);
    vec3 R_perp_eq =  eta_frac * (unit_vec + (cos_theta * v_norm));
    // find -(sqrt(1 - |R_perp_eq|^2 * norm)) --------- parallel to n'
    vec3  R_parallel_eq = -sqrt(fabs(1.0 - R_perp_eq.length_squared())) * v_norm;

    // add vectors perpendicular and parallel to unit_vec to find refraction vector
    return R_parallel_eq + R_perp_eq;
}

#endif