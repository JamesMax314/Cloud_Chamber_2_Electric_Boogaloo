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
#include <random>
#include <chrono>

#include "window.hpp"
#include "shaders.hpp"
#include "simulation.hpp"
#include "utils.hpp"
#include "camera.hpp"
#include "rays.hpp"
#include "track.hpp"
#include "drawables.hpp"


namespace app {
    class App {
        public:
        window::Window w;
        GLuint ParticleBufferA, ParticleBufferB, billboard_vertex_buffer, FBO, textureOut, depthOut;
        const int ParticleCount = 4;
        GLuint vao;
        drawable::Drawable square;
        shaders::Compute fancyShader;
        shaders::Shader quadShader;
        shaders::Shader rayShader;
        shaders::Shader basicShader;
    	shaders::Shader curlBakeShader;    
		shaders::Shader postProcessShader;
        shaders::Compute advectionShader;

        float pos = 0;
        glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);

        camera::Camera cam;

        simulation::Sim sim;
        simulation::DensitySim track_sim;
        rayMarch::RayMarch ray_marcher;
        rayMarch::RayMarch ray;

        drawable::Drawable boundingBox;

	std::default_random_engine rand_gen;
	std::uniform_real_distribution<double> uniform_dist;

        double t = 0;
        int drawFPS = 0;

        bool keys[GLFW_KEY_LAST] = { false };

        float motionSpeed = 0.01;

	float time = 0.0;

        App();

        void init();
        void initBuffers();
        void mainLoop();


    };
}
