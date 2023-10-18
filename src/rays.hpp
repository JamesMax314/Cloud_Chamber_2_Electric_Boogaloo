#pragma once

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_PURE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.hpp"
#include "window.hpp"

namespace rayMarch {
    typedef glm::vec3 Position;

    struct VertexNormal {
        glm::vec3 position;
        glm::vec3 normal;
    };

    struct VertexNormalText {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 texture;
    };

    class RayMarch {
        public:

        const GLfloat g_vertex_buffer_data[12] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
        };

        const GLfloat g_vertex_buffer_data_cube[112] = {
            -1.0f,-1.0f,-1.0f, // triangle 1 : begin
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, // triangle 1 : end
            1.0f, 1.0f,-1.0f, // triangle 2 : begin
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f, // triangle 2 : end
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f
        };

        unsigned int shaderProgramIndex;
        shaders::Shader* mCompShader;
        shaders::Shader* mRenderShader;

        std::vector<glm::vec3> mStartPos;
        std::vector<unsigned int> mIndices;

        // Intagers that are used to reference buffer arrays in gpu ram
        GLuint VAO;
        GLuint ParticleBufferA, ParticleBufferB, billboard_vertex_buffer, texture_buffer;

        int textureDim = 32;

        RayMarch();
        RayMarch(shaders::Shader *shader);
        RayMarch(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<rayMarch::Position> startPos);
        void init(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<rayMarch::Position> startPos);

        void update(window::Window* w);
        void fillBuffers();
        void loadUniforms();
        void draw(window::Window* w);
    };
}