#pragma once

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
#include "window.hpp"

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

        void clear();
        void activate();
        void deactivate();

    };
}
