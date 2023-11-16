#include "window.hpp"


void window::framebuffer_size_callback(int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

window::Window::Window()
{
    this->width = 1000;//EM_ASM_INT({return document.getElementById("canvas").width}, 100);
    this->height = 1000;//EM_ASM_INT({return document.getElementById("canvas").height}, 100);
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

	this->win = glfwCreateWindow(1000, 1000, "Clouds", nullptr, nullptr);
    if (!this->win) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(this->win);

    // Initialize GLEW
    glewExperimental = GL_TRUE;
	GLenum err = glewInit();
    if (err != GLEW_OK) {
        std::cerr << "Failed to initialize GLEW" << std::endl;
        glfwTerminate();
    }

    printf("Initialising WEBGL context!\n");
    // glfwSetFramebufferSizeCallback(win, framebuffer_size_callback);
}
window::Window::~Window(){
	glfwDestroyWindow(this->win);
}

//void window::Window::renderFrame()
//{
//    rebufferLights = true;
//
//    for (int i=0; i<meshes.size(); i++) {
//        if (meshes[i]->mShader) {
//            // Setup mesh uniforms
//
//            meshes[i]->draw(win);
//        }
//    }
//
//    glfwSwapBuffers(win);
//}

//void window::Window::addMesh(drawable::Drawable* mesh)
//{
//    meshes.push_back(mesh);
//}

float window::Window::getAspect()
{
    width = 1000;
    height = 1000;
    glViewport(0, 0, width, height);
    float aspect = (float)width / (float)height;
    return aspect;
}

void window::Window::makeContextCurrent()
{
	glfwMakeContextCurrent(this->win);
}

GLFWcursorposfun window::Window::setCursorPosCallback(GLFWcursorposfun callback){
    GLFWcursorposfun glfw_cursorpos_fun = glfwSetCursorPosCallback(this->win, callback);
    glfwSetInputMode(this->win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
	return glfw_cursorpos_fun;
}
	
GLFWkeyfun window::Window::setKeyCallback(GLFWkeyfun callback){ 
	return glfwSetKeyCallback(this->win, callback);
}

void window::Window::swapBuffers(){
    glfwSwapBuffers(this->win);
}

int window::Window::shouldClose(){
	return glfwWindowShouldClose(this->win);
}
