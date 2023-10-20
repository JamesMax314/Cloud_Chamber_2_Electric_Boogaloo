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
#include "rays.hpp"
#include "track.hpp"


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
        shaders::Shader rayShader;

        float pos = 0;
        glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);

        camera::Camera cam;

        simulation::Sim sim;
        rayMarch::RayMarch track_sim;
        // simulation::Sim track_sim;
        rayMarch::RayMarch ray;

        double t = 0;
        double now;
        int drawFPS = 0;

        bool keys[GLFW_KEY_LAST] = { false };

        float motionSpeed = 0.01;

	    float time = 0.0;

        App();

        void init();
        void mainLoop();


    };
}
