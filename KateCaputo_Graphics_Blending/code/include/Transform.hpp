#ifndef TRANSFORM_HPP
#define TRANSFORM_HPP

#include <glad/glad.h>
#include "glm/vec3.hpp"
#include "glm/gtc/matrix_transform.hpp"

// Stores transformations of 3D entities (cameras, objects, etc.)
class Transform{
public:

    // Constructor for a new transform
    Transform();

    // Destructor for the transform
    ~Transform();

    // Resets matrix transformations to the identity matrix
    void LoadIdentity();

    // Perform a translation of an object
    void Translate(float x, float y, float z);

    // Perform rotation about an axis
    void Rotate(float radians, float x, float y, float z);

    // Perform rotation about an axis
    void Scale(float x, float y, float z);
    
    // Returns the transformation matrix
    GLfloat* GetTransformMatrix();

    // Applys a transformation
    void ApplyTransform(Transform t);

    // Returns the transformation matrix
    glm::mat4 GetInternalMatrix();
    
private:
    // Stores the actual transformation matrix
    glm::mat4 m_modelTransformMatrix;
};


#endif
