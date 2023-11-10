#include "window.hpp"

GLFWwindow* window::setupWindow(int width, int height) {
        // Initialize GLFW and create a window

    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
    }

    // Necessary to use glsl 330 standard
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
#ifdef __APPLE__
    // std::cout << "I'm apple machine" << std::endl;
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif
    
    GLFWwindow* window = glfwCreateWindow(width/2, height/2, "Clouds", nullptr, nullptr);
    if (!window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
    if (glewInit() != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }

    printf("Initialising WEBGL context!\n");


    return window;
}

void window::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

window::Window::Window()
{
    this->width = 1000;//EM_ASM_INT({return document.getElementById("canvas").width}, 100);
    this->height = 1000;//EM_ASM_INT({return document.getElementById("canvas").height}, 100);
    win = setupWindow(width, height);
    // glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
}

void window::Window::renderFrame()
{
    rebufferLights = true;

    for (int i=0; i<meshes.size(); i++) {
        if (meshes[i]->mShader) {
            // Setup mesh uniforms

            meshes[i]->draw(win);
        }
    }

    glfwSwapBuffers(win);
}

void window::Window::addMesh(drawable::Drawable *mesh)
{
    meshes.emplace_back(mesh);
}

float window::Window::getAspect()
{
    // width = 1000;
    // height = 1000;
    glViewport(0, 0, width, height);
    float aspect = (float)width / (float)height;
    return aspect;
}

GLFWwindow *window::Window::getContext()
{
    return win;
}
