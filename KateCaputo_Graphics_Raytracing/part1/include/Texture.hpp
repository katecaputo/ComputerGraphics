#ifndef TEXTURE_H
#define TEXTURE_H

#include "vec3.hpp"
#include "Image.hpp"

#include <cmath>

class Texture {
    public:
        // erturns the color value of a specific point, given texture coords
        virtual vec3 value(double u, double v, const point3& p) const = 0;
};

// a class to represent when an object's texture is a solid color
class SolidColor : public Texture {
    public:
        // default, vector, and RGB constructor
        SolidColor() {}
        SolidColor(color c) : color_value(c) {}
        SolidColor(double r, double g, double b) : color_value(vec3(r, g, b)) {}

        // returns the same (given) color value for every pixel
        virtual vec3 value(double u, double v, const point3& p) const override {
            return color_value;
        }

    private:
        // the solid color of the texture
        color color_value;

};

// a class to represent when an object has a checkered texture
class Checkered : public Texture {
    public:
        // default, 2 vector, and 2 texture constructors
        // this allows for either colors or textures to serve as the checkers
        Checkered() {}
        Checkered(std::shared_ptr<Texture> e, std::shared_ptr<Texture> o) :
            even(e), odd(o) {}

        Checkered(vec3 c1, vec3 c2) :
            even(std::make_shared<SolidColor>(c1)),
            odd(std::make_shared<SolidColor>(c2)) {}

        Checkered(vec3 c1, std::shared_ptr<Texture> t) :
            even(std::make_shared<SolidColor>(c1)),
            odd(t) {}

        Checkered(std::shared_ptr<Texture> t, vec3 c2) :
            even(t),
            odd(std::make_shared<SolidColor>(c2)) {}

        // depending on where on the checker board we are, returns appropriate color value
        virtual vec3 value(double u, double v, const point3& p) const override {
            // sine wave creates an alternating pattern
            float sines = sin(10 * p.x()) * sin(10 * p.y()) * sin(10 * p.z());

            if (sines < 0) {
                return odd->value(u, v, p);
            } else {
                return even->value(u, v, p);
            }
        }
    
    public:
        std::shared_ptr<Texture> odd;
        std::shared_ptr<Texture> even;
};

// a class to represent when an object's texture is a given ppm image
class Image_Tex : public Texture {
    public:

        // default constructor and constructor when a texture file name is specified
        Image_Tex() {}

        Image_Tex(std::string filename)  {
            // creates a PPM image with the file name, passes data array as reference
            // for the PPM class to populate so it can be accessed here
            myPPM = PPM(filename, *&data);
        }

        // destructor for image rextures
        ~Image_Tex() {
            delete[] data;
        }

        // given UV coordinates, returns the appropriate color value from the given texture image
        virtual vec3 value(double u, double v, const vec3& p) const override {
            // if there is some issue with loading the data, returns blue
            if (data == nullptr) {
                return color(0, 0, 1);
            }
            
            // ensures that UV coordinates are between 0 and 1
            u = clamp(u, 0.0, 1.0);
            // flips the V coordinates so image is oriented properly
            v = 1.0 - clamp(v, 0.0, 1.0);

            // ensures that UV coordinates are ints, since we're working with
            // indicies of the pixel data array
            //      also scales U and V [0,1] to the appopriate image width and height
            int i = static_cast<int>(u * myPPM.getWidth());
            int j = static_cast<int>(v * myPPM.getHeight());

            // more exception handling
            if (i >= myPPM.getWidth()) {
                i = myPPM.getWidth() - 1;
            }
            if (i < 0.0) {
                i = 0;
            }
            if (j >= myPPM.getHeight()) {
                j = myPPM.getHeight() - 1;
            }
            if (j < 0.0) {
                j = 0;
            }

            // based on i and j, computes the k index of the pixel color in the data array
            int k = (j * 3 * myPPM.getWidth()) + (i * 3);

            // adjusts colors to be out of [0,255] instead of normalized to [0,1]
            const float color_scale = 1.0 / 255.0;

            // gets RBG values from data array
            uint8_t R = data[k];
            uint8_t G = data[k + 1];
            uint8_t B = data[k + 2]; 

            // a color made of the RBG values, scaled to be out of 255
            vec3 outC = color(color_scale * R, color_scale * G, color_scale * B);

            return std::make_shared<SolidColor>(outC)->value(u, v, p);
        }
        

    public:
        // ppm image to be used as texture
        PPM myPPM;
        // array to hold ppm data
        uint8_t* data;
};


#endif