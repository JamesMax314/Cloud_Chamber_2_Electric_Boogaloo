#pragma once

#include <vector>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_PURE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "window.hpp"
#include "shaders.hpp"

namespace drawable {
    typedef glm::vec3 Vertex;

    struct VertexNormal {
        glm::vec3 position;
        glm::vec3 normal;
    };

    struct VertexNormalText {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 texture;
    };

    class Drawable {
        public:
        unsigned int shaderProgramIndex;
        shaders::Shader* mShader;

        std::vector<glm::vec3> mVertices;
        std::vector<unsigned int> mIndices;

        // Intagers that are used to reference buffer arrays in gpu ram
        GLuint VAO;
        GLuint VBO; // Vertices
        GLuint EBO; // Indices
        GLuint NBO; // Normals

        GLuint drawType = GL_TRIANGLES;

        glm::mat4 model;
        glm::mat3 normalModel;

        Drawable();
        Drawable(shaders::Shader *shader);
        Drawable(shaders::Shader *shader, std::vector<glm::vec3> vertices, std::vector<unsigned int> indices);
        void init(shaders::Shader *shader, std::vector<glm::vec3> vertices, std::vector<unsigned int> indices);

        void fillBuffers();
        void loadUniforms();
        void draw(window::Window &w);
    };
}
