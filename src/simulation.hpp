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

namespace simulation {
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

    class Sim {
        public:

        const GLfloat g_vertex_buffer_data[12] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
        };

        unsigned int shaderProgramIndex;
        shaders::Shader* mCompShader;
        shaders::Shader* mRenderShader;

        std::vector<glm::vec3> mStartPos;
        std::vector<unsigned int> mIndices;

        // Intagers that are used to reference buffer arrays in gpu ram
        GLuint VAO;
        GLuint ParticleBufferA, ParticleBufferB, billboard_vertex_buffer;

        Sim();
        Sim(shaders::Shader *shader);
        Sim(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<simulation::Position> startPos);
        void init(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<simulation::Position> startPos);

        void update(window::Window* w);
        void fillBuffers();
        void loadUniforms();
        void draw(window::Window* w);
    };
}