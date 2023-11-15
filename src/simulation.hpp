#pragma once

#include <vector>
#include <array>
#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_PURE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaders.hpp"
#include "textures.hpp"
#include "frame.hpp"
#include "window.hpp"

namespace simulation {
    typedef glm::vec3 Position;

    struct VertexNormal {
        glm::vec3 position;
        glm::vec3 normal;
    };

    struct VertexNormalText {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec3 texture;
    };

    class Sim {
        public:

        const GLfloat g_vertex_buffer_data[12] = {
            -1.0f, -1.0f, 0.0f,
            1.0f, -1.0f, 0.0f,
            -1.0f, 1.0f, 0.0f,
            1.0f, 1.0f, 0.0f,
        };

        const GLfloat g_vertex_buffer_data_cube[112] = {
            -1.0f,-1.0f,-1.0f, // triangle 1 : begin
            -1.0f,-1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f, // triangle 1 : end
            1.0f, 1.0f,-1.0f, // triangle 2 : begin
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f, // triangle 2 : end
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            -1.0f,-1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            -1.0f,-1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f,-1.0f,
            1.0f,-1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f,-1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f,-1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f, 1.0f, 1.0f,
            -1.0f, 1.0f, 1.0f,
            1.0f,-1.0f, 1.0f
        };

        unsigned int shaderProgramIndex;
        shaders::Shader* mCompShader;
        shaders::Shader* mRenderShader;
        shaders::Shader* mBakeShader;

	unsigned int nVerts = 20000;
		int curl_noise_resolution = 1000;
        std::vector<glm::vec3> mStartPos;
        std::vector<unsigned int> mIndices;
        std::vector<simulation::Position> feedbackVec;
        std::vector<glm::vec3> newVerts;
		texture::Texture bakedCurlTex;

        frame::Frame frameBufferCloudDen;
        // Intagers that are used to reference buffer arrays in gpu ram
        GLuint VAO;
        GLuint ParticleBufferA, ParticleBufferB, billboard_vertex_buffer, baking_vertex_buffer;
	
	int isTrack;
	unsigned int current_index = 0;

        Sim();
        Sim(shaders::Shader *shader);
        Sim(shaders::Shader *compShader, shaders::Shader *renderShader, shaders::Shader *bakeShader, std::vector<simulation::Position> startPos, int isTrack);
        void init(shaders::Shader *compShader, shaders::Shader *renderShader, shaders::Shader *bakeShader, std::vector<simulation::Position> startPos, int isTrack);
		void bakeCurl(window::Window &w);

        void update(window::Window &w);
        void fillBuffers();
        void loadUniforms();
        void draw(window::Window &w);
    };

    class DensitySim: public Sim{
	public:

        void init(shaders::Shader *compShader, shaders::Shader *renderShader, shaders::Shader *bakeShader, std::vector<simulation::Position> startPos, int isTrack);

	const int N_stream_buffers = 4;
	std::array<GLuint, 4> StreamBufferID;	
	int write_buffer_index= 0;
	int read_buffer_index = 2;

	void update(window::Window &w);
	void update_feedbackVec();
	void addVerts(std::vector<simulation::Position>& new_verts);
	void fillBuffers();
	
    };


}
