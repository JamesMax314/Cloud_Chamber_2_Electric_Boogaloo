#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace camera {
    class Camera {
        public:
        glm::mat4 viewMatrix;
        glm::vec3 camPos;
        glm::vec3 cameraFacing; // Initial view direction
        glm::vec3 cameraUp;
        glm::vec3 cameraTarget;

        float motionSpeed;
        float rotSpeed;

        void move(float x, float y, float z);
        void rotate(float dx, float dy);
        void setViewMat(glm::mat4 view);
        glm::mat4 getViewMat();

        Camera();
    };
}