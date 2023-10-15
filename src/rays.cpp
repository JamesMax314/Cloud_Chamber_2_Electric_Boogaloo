#include "rays.hpp"

rayMarch::RayMarch::RayMarch()
{
}

rayMarch::RayMarch::RayMarch(shaders::Shader *shader)
{
}

void rayMarch::RayMarch::init(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<rayMarch::Position> startPos)
{
    mCompShader = compShader;
    mRenderShader = renderShader;
    mStartPos = startPos;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &ParticleBufferA);
    glGenBuffers(1, &ParticleBufferB);
    glGenBuffers(1, &billboard_vertex_buffer);
    fillBuffers();
}

rayMarch::RayMarch::RayMarch(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<rayMarch::Position> startPos)
{
    init(compShader, renderShader, startPos);
}

void rayMarch::RayMarch::update(window::Window* w)
{
    glfwMakeContextCurrent(w->getContext());

    // Set up the advection shader:
    mCompShader->activate();

    glBindVertexArray(VAO);

    // Turn off drawing
    glEnable(GL_RASTERIZER_DISCARD);

    // Specify the target buffer:
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);
    // Specify input format
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Set output buffer
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, ParticleBufferB);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Begin Reading data 
    glBeginTransformFeedback(GL_POINTS);

    // Do Calculation
    glDrawArrays(GL_POINTS, 0, mStartPos.size());

    // Close feedback
    glEndTransformFeedback();
    glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, 0);    

    glFlush();

    std::swap(ParticleBufferA, ParticleBufferB);
    glDisable(GL_RASTERIZER_DISCARD);

    glBindVertexArray(0);
}

void rayMarch::RayMarch::fillBuffers()
{
    // Bind all the model arrays to the appropriate buffers
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);
    glBufferData(GL_ARRAY_BUFFER, mStartPos.size()*sizeof(rayMarch::Position), &mStartPos.front(), GL_STREAM_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Create VBO for output on even-numbered frames and input on odd-numbered frames:
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferB);
    glBufferData(GL_ARRAY_BUFFER, mStartPos.size()*sizeof(rayMarch::Position), (void*)0, GL_STREAM_DRAW);

    // Bind rendering buffers
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void rayMarch::RayMarch::loadUniforms()
{
}

void rayMarch::RayMarch::draw(window::Window* w)
{
    glfwMakeContextCurrent(w->getContext());
    mRenderShader->activate();

    glBindVertexArray(VAO);

    // Bind quad to render
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Bind positions
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // float feedbackVec[6];
    // glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(feedbackVec), feedbackVec);

    // printf("%f %f %f %f %f %f\n", feedbackVec[0], feedbackVec[1], feedbackVec[2], feedbackVec[3], feedbackVec[4], feedbackVec[5]);
    
    //  Draw 1 quad (4 vertices) for every position
    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, mStartPos.size());

    glBindVertexArray(0);
}
