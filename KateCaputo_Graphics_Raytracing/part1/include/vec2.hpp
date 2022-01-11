#ifndef VEC2_H
#define VEC2_H

#include <cmath>

class vec2 {
    public:
        // vec2 default and input constructors
        vec2() {
            e[0] = 0;
            e[1] = 0;
        }

        vec2(double e0, double e1) {
            e[0] = e0;
            e[1] = e1;
        }

        // returns values of vectors
        double x() const {
            return e[0];
        }

        double y() const {
            return e[1];
        }

    public:
        double e[2];
};



#endif