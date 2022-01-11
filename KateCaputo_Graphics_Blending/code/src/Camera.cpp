#include "Camera.hpp"

#include "glm/gtx/transform.hpp"

#include <iostream>

void Camera::MouseLook(int mouseX, int mouseY){
    // Record our new position as a vector
    glm::vec2 newMousePosition(mouseX, mouseY);
    // Detect how much the mouse has moved since
    // the last time
    glm::vec2 mouseDelta = 0.01f*(newMousePosition-m_oldMousePosition);

    m_viewDirection = glm::mat3(glm::rotate(-mouseDelta.x, m_upVector)) * m_viewDirection;
    
    // Update our old position after we have made changes 
    m_oldMousePosition = newMousePosition;
}

// Moves the camera forward (into the scene)
void Camera::MoveForward(float speed){
    m_eyePosition.z -= speed;
}

// Moves the camera backwards (out of the scene)
void Camera::MoveBackward(float speed){
    m_eyePosition.z += speed;
}

// Moves the camera left
void Camera::MoveLeft(float speed){
    m_eyePosition.x -= speed;
}

// Moves the camera right
void Camera::MoveRight(float speed){
    m_eyePosition.x += speed;
}

// Moves the camera up
void Camera::MoveUp(float speed){
    m_eyePosition.y += speed;
}

// Moves the camera down
void Camera::MoveDown(float speed){
    m_eyePosition.y -= speed;
}

// Set the position for the camera
void Camera::SetCameraEyePosition(float x, float y, float z){
    m_eyePosition.x = x;
    m_eyePosition.y = y;
    m_eyePosition.z = z;
}

// Returns the X position of the camera
float Camera::GetEyeXPosition(){
    return m_eyePosition.x;
}

// Returns the Y position of the camera
float Camera::GetEyeYPosition(){
    return m_eyePosition.y;
}

// Returns the Z position of the camera
float Camera::GetEyeZPosition(){
    return m_eyePosition.z;
}

// CURRENTLY UNUSED
float Camera::GetViewXDirection(){
    return m_viewDirection.x;
}

// CURRENTLY UNUSED
float Camera::GetViewYDirection(){
    return m_viewDirection.y;
}

// CURRENTLY UNUSED
float Camera::GetViewZDirection(){
    return m_viewDirection.z;
}


Camera::Camera(){
    std::cout << "(Constructor) Created a Camera!\n";
	// Position the camera at the origin.
    m_eyePosition = glm::vec3(0.0f,0.0f, 0.0f);
	// looking down along the z-axis initially.
    m_viewDirection = glm::vec3(0.0f,0.0f, -1.0f);
	// up vector is along y axis
    m_upVector = glm::vec3(0.0f, 1.0f, 0.0f);
}

// Returns the world to view matric
glm::mat4 Camera::GetWorldToViewmatrix() const{
    return glm::lookAt( m_eyePosition, m_eyePosition + m_viewDirection, m_upVector);
}
