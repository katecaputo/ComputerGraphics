#ifndef COLOR_H
#define COLOR_H

#include "vec3.hpp"

#include <iostream>

// returns a string of the pixel's 3 color values
std::string write_color(color pixel_color, int samples_per_pixel);

#endif