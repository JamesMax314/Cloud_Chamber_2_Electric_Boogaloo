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

#include "window.hpp"
#include "shaders.hpp"

window::Window w;



void draw() {
    // printf("Ok \n");
}

std::vector<drawable::Vertex> vertices = {
    drawable::Vertex(0.5f,  0.5f, 0.0f),  // top right
    drawable::Vertex(0.5f, -0.5f, -0.0f),  // bottom right
    drawable::Vertex(-0.5f, -0.5f, 0.0f),  // bottom left
    drawable::Vertex(-0.5f,  0.5f, -0.0f)   // top left 
};
std::vector<unsigned int> indices = {  // note that we start from 0!
    0, 1, 3,   // first triangle
    1, 2, 3    // second triangle
}; 


int main()
{
    const char* vertexShaderFile = "../shaders/triangleVert.glsl";
    const char* fragmentShaderFile = "../shaders/triangleFrag.glsl";

    glfwMakeContextCurrent(w.getContext());

    glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);


    shaders::Shader triangleShader(vertexShaderFile, fragmentShaderFile);
    drawable::Drawable square(&triangleShader, vertices, indices);
    square.draw(w.getContext());
    glfwSwapBuffers(w.getContext());

    std::cout << glGetError() << std::endl;

    emscripten_set_main_loop(draw, 0, 1);

    return 0;
}