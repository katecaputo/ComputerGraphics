#include "OBJ.hpp"

#include <fstream>
#include <iostream>
#include <sstream>
#include <vector>
#include "glm/glm.hpp"
#include "glm/ext.hpp" 

// Creates an object that will have the texture which is passed in 
OBJ::OBJ(std::string texFilePath){

    // creates the 4 vertices of a square
    m_geometry.AddVertex(-1.0f,-1.0f,0.0f, 0.0f, 0.0f);  
    m_geometry.AddVertex(1.0f,-1.0f,0.0f, 1.0f, 0.0f);
    m_geometry.AddVertex(1.0f,1.0f,0.0f, 1.0f, 1.0f);
    m_geometry.AddVertex(-1.0f,1.0f,0.0f,0.0f, 1.0f);

    // makes a square using 2 triangles
    m_geometry.MakeTriangle(0, 1, 2);
    m_geometry.MakeTriangle(2, 3, 0);

    // actually generates the shape, it's normals, tangents, bitangents, etc.
    m_geometry.Gen();

    // Create Vertex Buffer Layout
    m_vertexBufferLayout.CreateNormalBufferLayout(m_geometry.GetBufferDataSize(),
                                        m_geometry.GetIndicesSize(),
                                        m_geometry.GetBufferDataPtr(),
                                        m_geometry.GetIndicesDataPtr());

    // Load our texture, as passed in to the constructor
    m_texture.LoadTexture(texFilePath);
        
    // Setup shaders
    std::string vertexShader = m_shader.LoadShader("./shaders/vert.glsl");
    std::string fragmentShader = m_shader.LoadShader("./shaders/frag.glsl");

    // Create our shader
    m_shader.CreateShader(vertexShader,fragmentShader);
}

// destructor deletes allocated memory
OBJ::~OBJ(){

}

void OBJ::Update(unsigned int screenWidth, unsigned int screenHeight, Camera* cam){
        m_texture.Unbind();
        m_vertexBufferLayout.Bind();
        m_texture.Bind(); 
        m_shader.Bind();

        // apply the texture by bounding it to slot 0
        m_shader.SetUniform1i("u_DiffuseMap",0); 
 
        // apply the 'view' matrix which creates perspective.
        m_projectionMatrix = glm::perspective(45.0f,(float)screenWidth/(float)screenHeight,0.1f,10.0f);

        // Set the uniforms in our current shader
        m_shader.SetUniformMatrix4fv("model",m_transform.GetTransformMatrix());
        m_shader.SetUniformMatrix4fv("view", &cam->GetWorldToViewmatrix()[0][0]);
        m_shader.SetUniformMatrix4fv("projection", &m_projectionMatrix[0][0]);

        // Create a 'light'
        m_shader.SetUniform3f("lightColor",1.0f,1.0f,1.0f);
        m_shader.SetUniform3f("lightPos", 0.0f, 0.0f, 0.0f);
        m_shader.SetUniform1f("ambientIntensity",1.0f);
        
}


void OBJ::Render(){
        // Select this objects buffer to render
        m_vertexBufferLayout.Bind();
        // Select this objects texture to render
        m_texture.Bind();
        // Select this objects shader to render
        m_shader.Bind();

	    //Render data
        glDrawElements(GL_TRIANGLES, m_geometry.GetIndicesSize(), GL_UNSIGNED_INT, nullptr);
}

// Returns the actual transform stored in our object which can then be modified
Transform& OBJ::GetTransform(){
    return m_transform; 
}

// returns the objects final position after translation
glm::vec3 OBJ::GetTranslationLocation() {
    return m_translationLocation;
}

// Set's the object's location for its translate transformation
void OBJ::SetTranslationLocation(float x_, float y_, float z_) {
    m_translationLocation.x = x_;
    m_translationLocation.y = y_;
    m_translationLocation.z = z_;
}