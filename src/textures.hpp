#pragma once

#include <emscripten.h>
#include <emscripten/val.h>
#include <emscripten/html5.h>
#include <emscripten/bind.h>
#define GLFW_INCLUDE_ES3
#include <GL/glew.h>
#include <GLFW/glfw3.h>
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

        GLuint getRef();
    };
}