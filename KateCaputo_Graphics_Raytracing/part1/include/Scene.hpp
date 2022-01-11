#ifndef SCENE_H
#define SCENE_H

#include "vec3.hpp"
#include "Ray.hpp"
#include "Hittable.hpp"

#include <vector>
#include <cmath>

class Scene {
    public:

        // constructor for scene
        Scene();

        // destructor for scene
        ~Scene();

        // add object to ray tracer
        void addObject(Hittable* s);

        // draw a color to the screen based on the ray
        vec3 color(const Ray& r, int depth, int& totalNumIntersections);

    private:
    
        //store sphere objects of scene
        std::vector<Hittable *> objs;
};


#endif