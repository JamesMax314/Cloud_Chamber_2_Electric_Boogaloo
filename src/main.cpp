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

app::App* application;
void app_draw(){
	application->mainLoop();
}

int main()
{
	application = new app::App();
	application->init();
	
    while (!application->w.shouldClose()) {
		emscripten_set_main_loop(&app_draw, 0, 1);
    }
    // window::Window mWindow = window::Window();
    // while (!glfwWindowShouldClose(mWindow.getContext())) {
    //     glfwSwapBuffers(mWindow.win);
    //     glfwPollEvents();

    // }
    glfwTerminate();

	delete(application);
    
    return 0;
}

