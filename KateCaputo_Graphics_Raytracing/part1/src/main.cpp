#include "Color.hpp"
#include "Ray.hpp"
#include "vec3.hpp"
#include "Hittable.hpp"
#include "Scene.hpp"
#include "Camera.hpp"
#include "Utility.hpp"
#include "Sphere.hpp"
#include "Triangle.hpp"
#include "OBJ.hpp"
#include "Texture.hpp"
#include "vec2.hpp"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <cmath>
#include <string>

int main() {

    // clock to keep track of how long program takes to run
    clock_t timeStart = clock();

    // other variables for tracking runtime
    static int totalNumRays = 0; 
    static int totalNumIntersections = 0; 

    // IMAGE WINDOW VAIRABLES
    const double aspect_ratio = 16.0 / 9.0;
    const int image_width = 400;
    const int image_height = static_cast<int>(image_width / aspect_ratio);
    const int samples_per_pixel = 50;
    const int max_depth = 50;

    // CAMERA
    point3 eye = point3(-1.5, 1, 4.0);
    point3 lookAt = point3(0, 0, -1);

    Camera cam(eye, lookAt, vec3(0,1,0), 90, aspect_ratio, 0.0, (eye - lookAt).length());

    std::vector<Camera> cam_vec;
    cam_vec.push_back(cam);

    // CHECKER TEXTURE
    std::shared_ptr<Checkered> checker =  std::make_shared<Checkered>(color(0.2, 0.3, 0.1), color(0.9, 0.9, 0.9));
    std::shared_ptr<Material> chMat = std::make_shared<Lambertian>(checker);

    //  OBJECT

    OBJ* o = new OBJ("./../../common/objects/house/house_obj.obj");

    // GET THE LIST OF TRIANGLES FROM THE OBJECT
    std::vector<Hittable *> listTri = o->getTriangles(); 

    // CREATE THE SCENE

    Scene myScene;

    for (int j = 0; j < listTri.size(); j++) {
        myScene.addObject(listTri[j]);
    }

    // RENDER THE IMAGE

    for (int k = 1; k <= cam_vec.size(); k++) {

        Camera tempCam = cam_vec[k - 1];

        std::ofstream outFile;
        // save output image as "output__.ppm" with __ being the image number
        std::string outFileName = "./output" + std::to_string(k) + ".ppm";
        outFile.open(outFileName);

        // PPM header
        outFile << "P3\n" << image_width << " " << image_height << "\n255\n";

        // PPM data
        for (int j = image_height - 1; j >= 0; --j) {
            // track progress
            std::cerr << "\rImage " << k << "/" << cam_vec.size() <<  " -- Scanlines remaining: " << j << ' ' << std::flush;

            for (int i = 0; i < image_width; ++i) {
                color pixel_color(0.0, 0.0, 0.0);
                for (int s = 0; s < samples_per_pixel; s++) {
                    double u = (i + random_double()) / (image_width - 1);
                    double v = (j + random_double()) / (image_height - 1);

                    // for each pixel, send out #samples_per_pixels of rays
                    // get this ray and find its color
                    // accumulate pixel color for every ray coming from pixel
                    Ray r = tempCam.get_ray(u,v);
                    totalNumRays++;
                    pixel_color =  pixel_color + myScene.color(r, max_depth, totalNumIntersections);
                }
                // for each pixel, write its color to the output file image
                outFile << write_color(pixel_color, samples_per_pixel);
            }
        }
        outFile.close();
    }

    std::cerr << "\nDone.\n";

    clock_t timeEnd = clock();

    std::cout << "Render time                                 : " << (float)(timeEnd - timeStart) / CLOCKS_PER_SEC << " (sec)" << std::endl; 
    std::cout << "Total number of intersections               : " << totalNumIntersections << std::endl; 
    std::cout << "Total number of rays cast                   : " << totalNumRays << std::endl; 

    return 0;
}