#ifndef OBJ_HPP
#define OBJ_HPP

#include <string>
#include <vector>

#include "Shader.hpp"
#include "Geometry.hpp"
#include "VertexBufferLayout.hpp"
#include "Texture.hpp"
#include "Camera.hpp"
#include "Transform.hpp"

class OBJ{
public:
    // Constructor loads what the filepath of the texture will be
    OBJ(std::string texFilePath);

    // Destructor clears any memory that has been allocated
    ~OBJ();

    // Updates and transformations applied to object
    void Update(unsigned int screenWidth, unsigned int screenHeight, Camera* cam);

    // Draws the object
    void Render();

    // Returns an objects transformations
    Transform& GetTransform();

    // gets the location of the object after transformations have been applied
    glm::vec3 GetTranslationLocation();

    // sets the translation location of the object
    // for this project, I am only translating objects once
    void SetTranslationLocation(float x, float y, float z);

private:

    // The object's shader
    Shader m_shader;

    // The object's vertex buffer layout
    VertexBufferLayout m_vertexBufferLayout;

    // The object's texture
    Texture m_texture;

    // The object's transformations
    Transform m_transform; 

    // The object's geometry
    Geometry m_geometry;

    // The 'camera' projection
    glm::mat4 m_projectionMatrix;

    // the object's final location after translation is applied
    glm::vec3 m_translationLocation;
};

#endif
