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
#include "app.hpp"

app::App application;

void draw() {
    application.mainLoop();
}

int main()
{
    application.init();
    while (!glfwWindowShouldClose(application.w.getContext())) {
		emscripten_set_main_loop(draw, 0, 1);
    }
    // window::Window mWindow = window::Window();
    // while (!glfwWindowShouldClose(mWindow.getContext())) {
    //     glfwSwapBuffers(mWindow.win);
    //     glfwPollEvents();

    // }
    glfwTerminate();
    
    return 0;
}
