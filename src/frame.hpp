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
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <memory>
#include <cmath>
#include <random>

#include "textures.hpp"

namespace frame
{
    class Frame 
    {
        public:
        GLuint FBO;
        texture::Texture* m_colourTexture;
        texture::Texture* m_depthTexture;

        Frame();
        void init(texture::Texture* colourTexture);
        void init(texture::Texture* colourTexture, texture::Texture* depthTexture);
        void checkConfig();

        void setRenderTexture(texture::Texture* colourTexture);
        void setRenderTexture(texture::Texture* colourTexture, texture::Texture* depthTexture);

    };
}