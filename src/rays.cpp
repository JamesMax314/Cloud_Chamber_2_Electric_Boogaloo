#include "rays.hpp"

rayMarch::RayMarch::RayMarch()
{
}

rayMarch::RayMarch::RayMarch(shaders::Shader *shader)
{
}

void rayMarch::RayMarch::init(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<simulation::Position> startPos, int isTrack)
{
    simulation::Sim::init(compShader, renderShader, startPos, isTrack);
    glGenTextures(1, &texture_buffer);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glBindVertexArray(0);

    fillBuffers();

    feedbackVec = std::vector<glm::vec3>(numParticlesPerTrack);
    texture3D = new float[textureDim];//std::vector<std::vector<std::vector<float>>>(textureDim, std::vector<std::vector<float>>(textureDim, std::vector<float>(textureDim, 0)));
}

rayMarch::RayMarch::RayMarch(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<simulation::Position> startPos, int isTrack)
{
    init(compShader, renderShader, startPos, isTrack);
}

void rayMarch::RayMarch::update(window::Window* w)
{
    simulation::Sim::update(w);

    // Get particle positions
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, (feedbackVec.size()-1)*sizeof(glm::vec3), feedbackVec.data());
    // printf("%f %f %f\n", feedbackVec[0][0], feedbackVec[0][1], feedbackVec[0][2]);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    minCorner = feedbackVec[0];
    maxCorner = feedbackVec[0];

    // Find bounding box
    for (int i=0; i<numParticlesPerTrack; i++) {
        for (int j=0; j<3; j++) {
            if (feedbackVec[i][j] < minCorner[j]) {
                minCorner[j] = feedbackVec[i][j];
            }
            if (feedbackVec[i][j] > maxCorner[j]) {
                maxCorner[j] = feedbackVec[i][j];
            }
        }
    }

    // Compute density texture
    glm::vec3 stepSize = (maxCorner - minCorner) / (float)(textureDim-1);
    // printf("Min %f, %f, %f \n", minCorner[0], minCorner[1], minCorner[2]);
    // printf("Max %f, %f, %f \n", maxCorner[0], maxCorner[1], maxCorner[2]);

    for (int i=0; i<numParticlesPerTrack; i++) {
        glm::ivec3 index3D;

        index3D = glm::floor((feedbackVec[i] - minCorner) / stepSize);
        texture3D[index3D.x + index3D.y*textureDim + index3D.z*textureDim*textureDim] += 0.001;
    }
}

void rayMarch::RayMarch::fillBuffers()
{
    // Bind all the model arrays to the appropriate buffers
    // glBindVertexArray(VAO);
    // Fragment buffers are handeled as textures

    glBindTexture(GL_TEXTURE_3D, texture_buffer);
    // Specify how to up/down sample
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, textureDim, textureDim, textureDim, 0, GL_RED, GL_FLOAT, NULL);

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
    
    //  Draw 1 quad (4 vertices) for every position
    glVertexAttribDivisor(0, 0);

    // Bind and populate Texture
    glActiveTexture(GL_TEXTURE0); // Texture unit 0
    glBindTexture(GL_TEXTURE_3D, texture_buffer);
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, textureDim, textureDim, textureDim, GL_RED, GL_FLOAT, texture3D);
    glUniform1i(glGetUniformLocation(mRenderShader->mProgram, "texture3D"), 0);

    // Set the max and min corners
    mRenderShader->setUniformVec("minPos", minCorner);
    mRenderShader->setUniformVec("maxPos", maxCorner);
    float fTextDim = (float)textureDim;
    mRenderShader->setUniform("texDim", fTextDim);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);

    glBindVertexArray(0);
}
