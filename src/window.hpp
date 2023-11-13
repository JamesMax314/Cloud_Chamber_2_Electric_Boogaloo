#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_PURE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "drawables.hpp"
#include "shaders.hpp"

namespace window {
    GLFWwindow* setupWindow(int width, int height);

    void framebuffer_size_callback(GLFWwindow* window, int width, int height);

    class Window {
        public:

        int height, width;

        GLFWwindow* win;
        GLuint lightUBO;

        bool rebufferLights;
        const int lightBuffbinding = 0;

        int lightingMode;
        int colourMode;

        float aspect;

        std::vector<drawable::Drawable*> meshes;

        Window();

        void renderFrame();

        void addMesh(drawable::Drawable *mesh);
        float getAspect();

        GLFWwindow* getContext();
    };
}