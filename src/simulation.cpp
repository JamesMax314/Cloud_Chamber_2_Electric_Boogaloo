#include "simulation.hpp"

simulation::Sim::Sim()
{
}

simulation::Sim::Sim(shaders::Shader *shader)
{
}

void simulation::Sim::init(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<simulation::Position> startPos, int isTrack)
{
    this->isTrack = isTrack;
    mCompShader = compShader;
    mRenderShader = renderShader;
    mStartPos = startPos;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &ParticleBufferA);
    glGenBuffers(1, &ParticleBufferB);
    glGenBuffers(1, &billboard_vertex_buffer);
    fillBuffers();

    feedbackVec = std::vector<glm::vec3>(startPos.size());
}

simulation::Sim::Sim(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<simulation::Position> startPos, int isTrack)
{
    init(compShader, renderShader, startPos, isTrack);
}

void simulation::Sim::update(window::Window* w)
{
    glfwMakeContextCurrent(w->getContext());

    // Set up the advection shader:
    mCompShader->activate();
    mCompShader->setUniform("is_track_vert", this->isTrack);

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

void simulation::Sim::fillBuffers()
{
    // Bind all the model arrays to the appropriate buffers
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);
    glBufferData(GL_ARRAY_BUFFER, mStartPos.size()*sizeof(simulation::Position), &mStartPos.front(), GL_STREAM_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Create VBO for output on even-numbered frames and input on odd-numbered frames:
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferB);
    glBufferData(GL_ARRAY_BUFFER, mStartPos.size()*sizeof(simulation::Position), (void*)0, GL_STREAM_DRAW);

    // Bind rendering buffers
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data_cube), g_vertex_buffer_data_cube, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void simulation::Sim::loadUniforms()
{
}

void simulation::Sim::updateFeedbackVec()
{
    // Get particle positions
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, (feedbackVec.size()-1)*sizeof(glm::vec3), feedbackVec.data());
    // printf("%f %f %f\n", feedbackVec[0][0], feedbackVec[0][1], feedbackVec[0][2]);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void simulation::Sim::draw(window::Window* w)
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

    // float feedbackVec[sizeof(simulation::Position)*mStartPos.size()];
    // glGetBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(feedbackVec), feedbackVec);

    // printf("%f %f %f %f %f %f\n", feedbackVec[0], feedbackVec[1], feedbackVec[2], feedbackVec[3], feedbackVec[4], feedbackVec[5]);
    
    //  Draw 1 quad (4 vertices) for every position
    glVertexAttribDivisor(0, 0);
    glVertexAttribDivisor(1, 1);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 36, mStartPos.size());

    glBindVertexArray(0);
}
