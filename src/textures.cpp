#include "textures.hpp"

texture::Texture::Texture()
{
}

void texture::Texture::initColour(int width, int height)
{
    m_width = width;
    m_height = height;

    glGenTextures(1, &m_textureRef);
    
    // Setup texture
    glBindTexture(GL_TEXTURE_2D, m_textureRef);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::Texture::initDepth(int width, int height)
{
    m_width = width;
    m_height = height;

    glGenTextures(1, &m_textureRef);

    glBindTexture(GL_TEXTURE_2D, m_textureRef);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT32F, m_width, m_height, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glBindTexture(GL_TEXTURE_2D, 0);
}

void texture::Texture::setInterp(GLuint interpMode)
{
    glBindTexture(GL_TEXTURE_2D, m_textureRef);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, interpMode);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, interpMode);
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint texture::Texture::getRef()
{
    return m_textureRef;
}
