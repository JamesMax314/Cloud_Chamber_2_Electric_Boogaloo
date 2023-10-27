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

#include "window.hpp"
#include "shaders.hpp"
#include "simulation.hpp"
#include "utils.hpp"
#include "camera.hpp"
#include "rays.hpp"
#include "track.hpp"
#include "drawables.hpp"
#include "frame.hpp"
#include "textures.hpp"


namespace app {
    class App {
        public:

        bool keys[GLFW_KEY_LAST] = { false };

        int drawFPS = 0;
        const int ParticleCount = 4;

        float pos = 0;
        float motionSpeed = 0.01;
	    float time = 0.0;

        double t = 0;
        double now;

        window::Window w;

        GLuint ParticleBufferA, ParticleBufferB, billboard_vertex_buffer, FBO, textureOut, depthOut;
        GLuint vao;

        shaders::Compute fancyShader;
        shaders::Shader quadShader;
        shaders::Shader rayShader;
        shaders::Shader basicShader;
        shaders::Shader postProcessShader;
        shaders::Shader densityCompShader;

        texture::Texture bubbleColourTex;
        texture::Texture bubbleDepthTex;

        frame::Frame frameBufferBackBubbles;

        glm::vec3 position = glm::vec3(0.0, 0.0, 0.0);

        camera::Camera cam;

        simulation::Sim sim;
        simulation::Sim track_sim;
        // simulation::DensitySim densitySim;
        
        rayMarch::RayMarch ray_marcher;
        rayMarch::RayMarch ray;

        drawable::Drawable boundingBox;
        drawable::Drawable square;

	    std::default_random_engine rand_gen;
	    std::uniform_real_distribution<double> uniform_dist;

        App();

        void init();
        void mainLoop();


    };
}
