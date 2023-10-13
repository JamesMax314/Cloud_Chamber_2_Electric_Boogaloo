#pragma once

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
#include "simulation.hpp"
#include "utils.hpp"
#include "camera.hpp"


namespace app {
    class App {
        public:
        window::Window w;
        GLuint ParticleBufferA, ParticleBufferB, billboard_vertex_buffer;
        const int ParticleCount = 4;
        GLuint vao;
        drawable::Drawable square;
        shaders::Compute fancyShader;
        shaders::Shader quadShader;
        camera::Camera cam;
        simulation::Sim sim;

        bool keys[GLFW_KEY_LAST] = { false };

        float motionSpeed = 0.1;

	    float time = 0.0;

        App();

        void init();
        void mainLoop();


    };
}
