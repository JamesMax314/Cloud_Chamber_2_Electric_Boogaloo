#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_PURE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <stdio.h>
#include <stdlib.h>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <cmath>
#include <random>

namespace texture
{
    class Texture 
    {
        public:
        GLuint m_textureRef;

        int m_width, m_height;

        Texture();
        void initColour(int width, int height);
        void initDepth(int width, int height);
        void setInterp(GLuint interpMode);
        void setColour(glm::vec4 colour);

        GLuint getRef();
    };
}
