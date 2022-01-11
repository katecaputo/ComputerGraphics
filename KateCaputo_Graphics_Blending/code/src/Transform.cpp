#include "Transform.hpp"

// By default, all transform matrices are also identity matrices
Transform::Transform(){
    LoadIdentity();
}

// destructor
Transform::~Transform(){

}

// Resets the model transform as the identity matrix.
void Transform::LoadIdentity(){
    m_modelTransformMatrix = glm::mat4(1.0f);
}

// Translates the model according to the given coords
void Transform::Translate(float x, float y, float z){
    m_modelTransformMatrix = glm::translate(m_modelTransformMatrix,glm::vec3(x,y,z));                            
}

// Rotates the model according to the given coords
void Transform::Rotate(float radians, float x, float y, float z){
    m_modelTransformMatrix = glm::rotate(m_modelTransformMatrix, radians,glm::vec3(x,y,z));        
}

// Scales the model according to the given coords
void Transform::Scale(float x, float y, float z){
    m_modelTransformMatrix = glm::scale(m_modelTransformMatrix,glm::vec3(x,y,z));        
}

// Returns the actual transform matrix
GLfloat* Transform::GetTransformMatrix(){
    return &m_modelTransformMatrix[0][0];
}


// Get the raw internal matrix from the class
glm::mat4 Transform::GetInternalMatrix(){
    return m_modelTransformMatrix;
}

// Applys a transformation to the model matrix
void Transform::ApplyTransform(Transform t){
    m_modelTransformMatrix = t.GetInternalMatrix();
}



