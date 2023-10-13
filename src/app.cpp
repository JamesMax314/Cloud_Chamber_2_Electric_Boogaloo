#include "app.hpp"

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

GLfloat g_vertex_buffer_data[12] = {
            -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            -0.5f, 0.5f, 0.0f,
            0.5f, 0.5f, 0.0f,
        };

app::App::App()
{
}

void app::App::init()
{
    const char* vertexShaderFile = "../shaders/triangleVert.glsl";
    const char* fragmentShaderFile = "../shaders/triangleFrag.glsl";

    const char* sqrtShaderFile = "../shaders/sqrtVert.glsl";
    const char* fancyShaderFile = "../shaders/fancyVert.glsl";

    const char* quadFrag = "../shaders/quadFrag.glsl";
    const char* quadVert = "../shaders/quadVert.glsl";

    glfwMakeContextCurrent(w.getContext());

    fancyShader.init(fancyShaderFile);
    GLuint ParticleAdvectProgram = fancyShader.mProgram;
    glUseProgram(ParticleAdvectProgram);


    // glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT);

    std::vector<drawable::Vertex> seed_particles(ParticleCount, drawable::Vertex(0, 0, 0));
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


    // // // // // //
    // Quad rendering
    printf("Compiling quad shader\n");
    quadShader.init(quadVert, quadFrag);

    // GLuint VAO;
    // glGenVertexArrays(1, &VAO);
    // glBindVertexArray(VAO);

    // Buffer to hold quad mesh for rendering each particle
    glGenBuffers(1, &billboard_vertex_buffer);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    // glBindVertexArray(0);

    glUseProgram(quadShader.mProgram);

    // 1rst attribute buffer : vertices
    // glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

// 2nd attribute buffer : positions of particles' centers
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void *) 0);
    

    // glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(feedbackVec), feedbackVec);

    // printf("%f %f %f\n", feedbackVec[0], feedbackVec[1], feedbackVec[2]);


//  Draw a square for each bubble
    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 2);
    // glDrawArrays(GL_TRIANGLES, 0, 4);

    shaders::Shader triangleShader(vertexShaderFile, fragmentShaderFile);
    square.init(&quadShader, vertices, indices);
    square.draw(w.getContext());

    glfwSwapBuffers(w.getContext());
}

void app::App::mainLoop()
{
    glfwMakeContextCurrent(w.getContext());

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glClear(GL_COLOR_BUFFER_BIT);

    GLuint ParticleAdvectProgram = fancyShader.mProgram;

    // Set up the advection shader:
    glUseProgram(fancyShader.mProgram);

    // Specify the source buffer:
    glEnable(GL_RASTERIZER_DISCARD);
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);

    // Need to actually pass the data to the shader

    // Specify the target buffer:
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);
    // Specify input format
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, nullptr);


    // Set output buffer
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, ParticleBufferB);
    glBindBuffer(GL_ARRAY_BUFFER, 0);


//     // Perform GPU advection:
    glBeginTransformFeedback(GL_POINTS);

//     // Do Calculation
    glDrawArrays(GL_POINTS, 0, ParticleCount);
    glEndTransformFeedback();
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);    

    glFlush();

    float feedbackVec[3];
    // glGetBufferSubData(GL_TRANSFORM_FEEDBACK_BUFFER, 0, sizeof(feedbackVec), feedbackVec);

    // printf("%f %f %f\n", feedbackVec[0], feedbackVec[1], feedbackVec[2]);

//     // Swap the A and B buffers for ping-ponging, then turn the rasterizer back on:
    std::swap(ParticleBufferA, ParticleBufferB);
    glDisable(GL_RASTERIZER_DISCARD);

//     glBindBuffer(GL_ARRAY_BUFFER, 0);




// // // // // // // // //
// // Rendering
    glUseProgram(quadShader.mProgram);

    // glBindVertexArray(vao);

    // 1rst attribute buffer : vertices
    // glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

// 2nd attribute buffer : positions of particles' centers
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(feedbackVec), feedbackVec);

    printf("%f %f %f\n", feedbackVec[0], feedbackVec[1], feedbackVec[2]);


// //  Draw a square for each bubble
    glVertexAttribDivisor(0, 0); // particles vertices : always reuse the same 4 vertices -> 0
    glVertexAttribDivisor(1, 1); // positions : one per quad (its center) -> 1

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);
    // glBindVertexArray(0);
    // printf("Frame\n");
    // square.draw(w.getContext());

    glfwSwapBuffers(w.getContext());



    // glfwSwapBuffers(w.getContext());



}
