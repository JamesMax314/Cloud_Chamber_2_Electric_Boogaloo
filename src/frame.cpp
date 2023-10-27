#include "frame.hpp"

frame::Frame::Frame()
{
}

void frame::Frame::init(texture::Texture* colourTexture)
{
    m_colourTexture = colourTexture;
    glGenFramebuffers(1, &FBO);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colourTexture->getRef(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    checkConfig();
}

void frame::Frame::init(texture::Texture* colourTexture, texture::Texture* depthTexture)
{
    m_colourTexture = colourTexture;
    m_depthTexture = depthTexture;
    glGenFramebuffers(1, &FBO);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colourTexture->getRef(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->getRef(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    checkConfig();
}

void frame::Frame::checkConfig()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Check configuration
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        switch (status) {
            case GL_FRAMEBUFFER_UNDEFINED:
                printf("GL_FRAMEBUFFER_UNDEFINED\n");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                printf("GL_FRAMEBUFFER_UNSUPPORTED\n");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFE\n");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                printf("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n");
                break;
            // Handle other possible status codes as needed
            default:
                printf("Unknown Error\n");
                break;
        }
    }
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void frame::Frame::setRenderTexture(texture::Texture *colourTexture)
{
    m_colourTexture = colourTexture;
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colourTexture->getRef(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void frame::Frame::setRenderTexture(texture::Texture *colourTexture, texture::Texture *depthTexture)
{
    m_colourTexture = colourTexture;
    m_depthTexture = depthTexture;

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_colourTexture->getRef(), 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, m_depthTexture->getRef(), 0);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void frame::Frame::clear()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

void frame::Frame::activate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
}

void frame::Frame::deactivate()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}
