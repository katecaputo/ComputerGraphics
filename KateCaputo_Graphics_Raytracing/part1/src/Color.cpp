#include "Color.hpp"
#include "Utility.hpp"

#include <iostream>
#include <cmath>

// returns a pixels color values as a string so it can be written to the output file
std::string write_color(color pixel_color, int sames_per_pixel) {
    // store RGB vars
    double r = pixel_color.x();
    double g = pixel_color.y();
    double b = pixel_color.z();

    // div color by num samples & gamma correct
    //      gamma correct for gamma = 2.0
    //      raise color to 1/gamma or 1/2
    //      things to the power of 1/2 are just square rooted
    double scale = 1.0 / sames_per_pixel;
    r = sqrt(r * scale);
    g = sqrt(g * scale);
    b = sqrt(b * scale);

    // return the translated [0,255] value of each color component as one string
    return std::to_string(static_cast<int>(256 * clamp(r, 0.0, 0.999))) + ' '
        + std::to_string(static_cast<int>(256 * clamp(g, 0.0, 0.999))) + ' '
        + std::to_string(static_cast<int>(256 * clamp(b, 0.0, 0.999))) + '\n';
}