#pragma once

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_PURE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "simulation.hpp"
#include "shaders.hpp"
#include "window.hpp"

namespace rayMarch {

    class RayMarch: public simulation::Sim {
        public:

        std::vector<unsigned int> mIndices;

        // Intagers that are used to reference buffer arrays in gpu ram
        GLuint texture_buffer;

        glm::vec3 minCorner;
        glm::vec3 maxCorner;

        int numParticlesPerTrack = 10000;
        int textureDim = 64;
        std::vector<glm::vec3> feedbackVec;
        float* texture3D;

        RayMarch();
        RayMarch(shaders::Shader *shader);
        RayMarch(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<simulation::Position> startPos, int isTrack);
        void init(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<simulation::Position> startPos, int isTrack);

        void update(window::Window* w);
        void fillBuffers();
        void loadUniforms();
        void draw(window::Window* w);
    };
}
