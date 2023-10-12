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

    const char* sqrtShaderFile = "../shaders/sqrtVert.glsl";
    const char* fancyShaderFile = "../shaders/fancyVert.glsl";

    glfwMakeContextCurrent(w.getContext());

    shaders::Compute fancyShader(fancyShaderFile);
    GLuint ParticleAdvectProgram = fancyShader.mProgram;

    // glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint ParticleBufferA, ParticleBufferB;
    const int ParticleCount = 50;
    std::vector<drawable::Vertex> seed_particles(ParticleCount, drawable::Vertex(10, 0, 0));
    printf("Test: %f %f %f\n", seed_particles[0][0], seed_particles[0][1], seed_particles[0][2]);

    // Create VBO for input on even-numbered frames and output on odd-numbered frames:
    glGenBuffers(1, &ParticleBufferA);
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);
    glBufferData(GL_ARRAY_BUFFER, seed_particles.size()*sizeof(drawable::Vertex), &seed_particles.front(), GL_STREAM_DRAW);
    // glBindBuffer(GL_ARRAY_BUFFER, 0);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);

    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Create VBO for output on even-numbered frames and input on odd-numbered frames:
    glGenBuffers(1, &ParticleBufferB);
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferB);
    glBufferData(GL_ARRAY_BUFFER, seed_particles.size()*sizeof(drawable::Vertex), nullptr, GL_STREAM_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Set up the advection shader:
    glUseProgram(ParticleAdvectProgram);

    // Specify the source buffer:
    // glEnable(GL_RASTERIZER_DISCARD);
    // glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);

    // Need to actually pass the data to the shader

    // Specify the target buffer:
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, ParticleBufferB);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


    // Perform GPU advection:
    glBeginTransformFeedback(GL_POINTS);
    // glBindTexture(GL_TEXTURE_3D, VelocityTexture.Handle);
    glDrawArrays(GL_POINTS, 0, ParticleCount);
    glEndTransformFeedback();

    glFlush();

    float feedbackVec[3];
    glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedbackVec), feedbackVec);

    printf("%f %f %f\n", feedbackVec[0], feedbackVec[1], feedbackVec[2]);

    // Swap the A and B buffers for ping-ponging, then turn the rasterizer back on:
    std::swap(ParticleBufferA, ParticleBufferB);
    // glDisable(GL_RASTERIZER_DISCARD);







    shaders::Shader sqrtShader(sqrtShaderFile);
    GLuint program = sqrtShader.mProgram;

    GLuint vao;
    glGenVertexArrays(1, &vao);
    glBindVertexArray(vao);

    GLfloat data[] = { 1.0f, 2.0f, 3.0f, 4.0f, 5.0f };

    GLuint vbo;
    glGenBuffers(1, &vbo);
    glBindBuffer(GL_ARRAY_BUFFER, vbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), data, GL_STATIC_DRAW);

    GLint inputAttrib = glGetAttribLocation(program, "inValue");
    glEnableVertexAttribArray(inputAttrib);
    glVertexAttribPointer(inputAttrib, 1, GL_FLOAT, GL_FALSE, 0, 0);

    GLuint tbo;
    glGenBuffers(1, &tbo);
    glBindBuffer(GL_ARRAY_BUFFER, tbo);
    glBufferData(GL_ARRAY_BUFFER, sizeof(data), nullptr, GL_STATIC_READ);
    
    glEnable(GL_RASTERIZER_DISCARD);

    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, tbo);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    glBeginTransformFeedback(GL_POINTS);

    glDrawArrays(GL_POINTS, 0, 5);

    glEndTransformFeedback();

    glFlush();

    GLfloat feedback[5];
    glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedback), feedback);

    printf("%f %f %f %f %f\n", feedback[0], feedback[1], feedback[2], feedback[3], feedback[4]);

    glDisable(GL_RASTERIZER_DISCARD);

    shaders::Shader triangleShader(vertexShaderFile, fragmentShaderFile);
    drawable::Drawable square(&triangleShader, vertices, indices);
    // square.draw(w.getContext());
    glfwSwapBuffers(w.getContext());

    emscripten_set_main_loop(draw, 0, 1);

    return 0;
}