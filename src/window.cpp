#include "window.hpp"

GLFWwindow* window::setupWindow() {
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

    GLFWwindow* window = glfwCreateWindow(800, 600, "OpenGL 3D Cube", nullptr, nullptr);
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

    return window;
}

window::Window::Window()
{
    win = setupWindow();
    glGenBuffers(1, &lightUBO);
    bufferLights();
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

void window::Window::addMesh(drawable::GroundMesh *mesh)
{
    meshes.emplace_back(mesh);
}

GLFWwindow *window::Window::getContext()
{
    return win;
}
