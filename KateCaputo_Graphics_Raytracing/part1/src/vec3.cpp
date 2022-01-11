#include "vec3.hpp"

#include <cmath>

// default vec3 constructor
vec3::vec3()  {
    e[0] = 0;
    e[1] = 0;
    e[2] = 0;
}

// vec3 constructor with given inputs 
vec3::vec3(double e0, double e1, double e2) {
    e[0] = e0;
    e[1] = e1;
    e[2] = e2;
}

// returns 1st (x) value of vector
double vec3::x() const {
    return e[0];
}

// returns 2nd (y) value of vector
double vec3::y() const {
    return e[1];
}

// returns 3rd (z) value of vector
double vec3::z() const {
    return e[2];
}

// operators for vec3s (largely unused)
vec3 vec3::operator-() const {
    return vec3(-e[0], -e[1], -e[2]);
}

double vec3::operator[](int i) const {
    return e[i];
}

double& vec3::operator[](int i) { 
    return e[i]; 
}

vec3& vec3::operator+=(const vec3 &v) {
    e[0] += v.e[0];
    e[1] += v.e[1];
    e[2] += v.e[2];
    return *this;
}

vec3& vec3::operator*=(const double t) {
    e[0] *= t;
    e[1] *= t;
    e[2] *= t;
    return *this;
}

vec3& vec3::operator/=(const double t) {
    return *this *= 1/t;
}

// returns length of the vector
double vec3::length() const {
    return sqrt(length_squared());
}

// returns the length of a vector squared
double vec3::length_squared() const {
    return e[0]*e[0] + e[1]*e[1] + e[2]*e[2];
        }

// is the vector near zero in all dimensions?
bool vec3::near_zero() const {
    const float s = 1e-8;
    return (fabs(e[0]) < s) && (fabs(e[1]) < s) && (fabs(e[2]) < s);
}