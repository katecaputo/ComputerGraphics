#include "Scene.hpp"
#include "Sphere.hpp"
#include "vec3.hpp"
#include "Hittable.hpp"

#include <cmath>


// scene constructor
Scene::Scene() {
}

// scene destructor
Scene::~Scene() {
    for (int i = 0; i < objs.size(); i++) {
        delete objs[i];
    }
}

// add a hittable object to a scene
void Scene::addObject(Hittable* s) {
    objs.push_back(s);
}

// draws a color to a scene based on the given ray
vec3 Scene::color(const Ray& r, int depth, int& totalNumIntersections) {

    //final output color
    vec3 outColor(0.0f, 0.0f, 0.0f);

    // keep track of what is hit
    point3 recPoint;
    vec3 recNorm;
    double recT;
    std::shared_ptr<Material> recMtl;
    double recU;
    double recV;
    
    // if exceeded ray bounce limit, no more light is gathered
    if (depth <= 0) {
        return outColor;
    }

    // temporary variables
    point3 tempPoint;
    vec3 tempNorm;
    double tempT;
    std::shared_ptr<Material> tempMtl;
    double tempU;
    double tempV;

    // have there been any hits?
    bool any_hits = false;
    // distance of the closest intersection so far; intialized high
    double closest_so_far = 10000.0f;

    // iterate through every object in the scene
    for (unsigned int i = 0; i < objs.size(); i++) {
        // is tbis object hit? AND is it the closest hit so far?
        if (objs[i]->hit(r, .001, closest_so_far, tempPoint,
            tempNorm, tempT, tempMtl, tempU, tempV)) {

            any_hits = true;
            // update records
            closest_so_far = tempT;
            recPoint = tempPoint;
            recNorm = tempNorm;
            recT = tempT;
            recMtl = tempMtl;
            recU = tempU;
            recV = tempV;
            totalNumIntersections++;
        }
    }

    // if successful intersection, scatter the light
    if (any_hits) {
        // scattered will be the accumulated ray
        Ray scattered;
        vec3 attn;
 
        // recrurively accumulate final output color by looking at scattered ray
        // will depend on type of material
        if (recMtl->scatter(r, recPoint, recNorm, attn, scattered, recU, recV)) {
            return attn * color(scattered, depth - 1, totalNumIntersections);
        } else {
            return vec3(0.0f, 0.0f, 0.0f);
        }
    }

    // if no hits, render background
    vec3 unit_direction = unit_vector(r.direction());
    //      background shading changes based on vertical (y) direction
    double l = 0.55 * (unit_direction.y() + 1.0);
    return (1.0 - l) * vec3(1.0, 1.0, 1.0) + l * vec3(0.3, 0.0, 0.6);

    return vec3(1.0, 1.0, 1.0);
}