#include "camera.hpp"
#include <iostream>

void camera::Camera::move(float x, float y, float z)
{
    glm::vec3 worldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 cameraFrontNoUp = glm::vec3(cameraFront[0], 0.0f, cameraFront[2]); // Allows motion only in the plain 
    camPos += -x*glm::cross(cameraFrontNoUp, worldUp) + -y*worldUp + z*cameraFrontNoUp;

    cameraTarget = camPos + cameraFront; // Look just infront of camera
    viewMatrix = glm::lookAt(camPos, cameraTarget, cameraUp);
}

void camera::Camera::rotate(float dx, float dy)
{
    float angX = -dx*rotSpeed;
    float angY = -dy*rotSpeed;

    glm::mat4 yawRotation = glm::rotate(glm::mat4(1.0f), angX, cameraUp);
    cameraFront = glm::vec3(yawRotation * glm::vec4(cameraFront, 0.0f));

    // Pitch (up/down rotation)
    glm::mat4 pitchRotation = glm::rotate(glm::mat4(1.0f), angY, glm::cross(cameraFront, cameraUp));
    cameraFront = glm::vec3(pitchRotation * glm::vec4(cameraFront, 0.0f));

    cameraTarget = camPos + cameraFront;
    viewMatrix = glm::lookAt(camPos, cameraTarget, cameraUp);
}

void camera::Camera::setViewMat(glm::mat4 view)
{
    viewMatrix = view;
}

camera::Camera::Camera()
{
    // Set camera to origin
    camPos = glm::vec3(0.0f, 0.0f, 0.0f);
    cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    cameraTarget = camPos + cameraFront;
    cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    motionSpeed = 0.1f;
    rotSpeed = 0.005f;

    viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 1.0f), glm::vec3(0.0f, 0.0f, -2.0f), glm::vec3(0.0f, 1.0f, 0.0f));

}
