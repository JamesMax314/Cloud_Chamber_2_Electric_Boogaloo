#include "camera.hpp"
#include <iostream>

void camera::Camera::move(float x, float y, float z)
{
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraFrontNoUp = glm::vec3(cameraFacing[0], 0.0f, cameraFacing[2]); // Allows motion only in the plain 
    camPos += -x*glm::cross(cameraFrontNoUp, worldUp) + -y*worldUp + z*cameraFrontNoUp;

    cameraTarget = camPos + cameraFacing; // Look just infront of camera
    // viewMatrix = glm::lookAt(camPos, cameraTarget, cameraUp);
}

void camera::Camera::rotate(float dx, float dy)
{
    float angX = -dx*rotSpeed;
    float angY = -dy*rotSpeed;

    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), angX, cameraUp);
    cameraFacing = glm::vec3(yawRotation * glm::vec4(cameraFacing, 0.0f));

    // Pitch (up/down rotation)
    glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), angY, glm::cross(cameraFacing, cameraUp));
    cameraFacing = glm::vec3(pitchRotation * glm::vec4(cameraFacing, 0.0f));

    // cameraTarget = camPos + cameraFacing;
    // viewMatrix = glm::lookAt(camPos, cameraTarget, cameraUp);
}

void camera::Camera::setViewMat(glm::mat4 view)
{
    viewMatrix = view;
}

glm::mat4 camera::Camera::getViewMat()
{
    glm::vec3 cameraTarget = camPos + cameraFacing;
    glm::mat4 viewMatrix = glm::lookAt(camPos, cameraTarget, cameraUp);
    return viewMatrix;
}

camera::Camera::Camera()
{
    // Set camera to origin
    camPos = glm::vec3(0.0f, 0.0f, -4.0f);
    cameraFacing = glm::vec3(0.0f, 0.0f, 1.0f);
    cameraTarget = camPos + cameraFacing;
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    motionSpeed = 0.1f;
    rotSpeed = 0.005f;

    // viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f));

}
