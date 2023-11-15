#pragma once

#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_PURE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.hpp"

namespace window {

    void framebuffer_size_callback(int width, int height);

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

        //std::vector<drawable::Drawable*> meshes;

        Window();

        void renderFrame();

        //void addMesh(drawable::Drawable* mesh);
        float getAspect();

		void makeContextCurrent();
		GLFWcursorposfun setCursorPosCallback(GLFWcursorposfun callback );
		GLFWkeyfun setKeyCallback(GLFWkeyfun callback); 
		void swapBuffers();
		int shouldClose();
    };
}
