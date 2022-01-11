#ifndef OBJ_HPP
#define OBJ_HPP

#include <string>
#include <vector>
#include <cmath>

#include "Hittable.hpp"

class OBJ{
public:
    // Constructor loads a filename with the .obj extension
    OBJ(std::string fileName);

    // Destructor clears any memory that has been allocated
    ~OBJ();

    // returns the list of Triangles in the object
    std::vector<Hittable *> getTriangles() {
        return m_triangles;
    }

private:

    // all the Vertex Data (x,y,z,s,t) stored in one array of floats
    std::vector<float> m_vertices;

    // all the index data, stored in an array of ints
    std::vector<unsigned int> m_indices;

    // all the triangles that make up the obejct
    std::vector<Hittable *> m_triangles;
};

#endif