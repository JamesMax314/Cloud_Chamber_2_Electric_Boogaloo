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

void texture::Texture::setColour(glm::vec4 colour)
{
    std::vector<GLfloat> textureData(m_width*m_height*sizeof(glm::vec4));
    for (int i=0; i<m_width; i++){
        for (int i=0; i<m_height; i++){
            textureData[0] = colour.r;
            textureData[1] = colour.g;
            textureData[2] = colour.b;
            textureData[3] = colour.a;
        }
    }
    
    glBindTexture(GL_TEXTURE_2D, m_textureRef);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, m_width, m_height, GL_RGBA, GL_UNSIGNED_BYTE, textureData.data());
    glBindTexture(GL_TEXTURE_2D, 0);
}

GLuint texture::Texture::getRef()
{
    return m_textureRef;
}
