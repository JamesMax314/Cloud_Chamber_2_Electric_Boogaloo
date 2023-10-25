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

    class RayMarch {
        public:

        const GLfloat g_vertex_buffer_data[12] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
        };

        std::vector<unsigned int> mIndices;

        // Intagers that are used to reference buffer arrays in gpu ram
        GLuint VAO, FBO, texture_buffer, billboard_vertex_buffer, cloudTex;

        shaders::Shader* mCompShader;
        shaders::Shader* mRenderShader;
        shaders::Shader* mPostProcessShader;

        glm::vec3 minCorner;
        glm::vec3 maxCorner;

        int numParticlesPerTrack = 5000;
        int textureDim = 64;
        int renderWidth = 1080;
        int renderHeight = 720;
        float maxTexVal = 20;
        float* texture3D;

        RayMarch();
        RayMarch(shaders::Shader *shader);
        RayMarch(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<simulation::Position> startPos, int isTrack);
        void init(shaders::Shader *compShader, shaders::Shader *renderShader, shaders::Shader *postProcessShader);

        void genFBO();
        void update(std::vector<glm::vec3> &feedbackVec);
        void fillBuffers();
        void loadUniforms();
        void draw(window::Window* w);
        void genMask(window::Window* w, GLuint backgroundTexture, GLuint backgroundDepth);
        void draw(window::Window* w, GLuint backgroundTexture, GLuint backgroundDepth);
    };
}
