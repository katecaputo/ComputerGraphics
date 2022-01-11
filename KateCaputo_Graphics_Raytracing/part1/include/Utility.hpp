#ifndef UTIL_H
#define UTIL_H

#include <cmath>

// Utility Functions

// will cut off unneeded parts of camera frame
inline double clamp(double x, double min, double max) {
    if (x < min) return min;
    if (x > max) return max;
    return x;
}

// generates a random double between 0 (inclusive) and 1 (exclusive)
inline double random_double() {
    return rand() / (RAND_MAX + 1.0);
}

// generates a random double between a given minimun (inclusive) and max (exclusive)
inline double random_double(double min, double max) {
    return min + (max-min)*random_double();
}

#endif