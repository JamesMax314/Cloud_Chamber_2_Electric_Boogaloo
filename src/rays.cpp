#include "rays.hpp"

rayMarch::RayMarch::RayMarch()
{
}

rayMarch::RayMarch::RayMarch(shaders::Shader *shader)
{
}

void rayMarch::RayMarch::init(shaders::Shader *compShader, shaders::Shader *renderShader, shaders::Shader *postProcessShader)
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &billboard_vertex_buffer);
    glGenTextures(1, &texture_buffer);

    mCompShader = compShader;
    mRenderShader = renderShader;
    mPostProcessShader = postProcessShader;

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glBindVertexArray(0);

    texture3D.insert(texture3D.begin(), flattenedDim, 0.0);

    cloud_texture.initColour(renderWidth, renderHeight);
    cloud_frame.init(&cloud_texture);

    fillBuffers();
}

rayMarch::RayMarch::RayMarch(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<simulation::Position> startPos, int isTrack)
{
    // init(compShader, renderShader, startPos, isTrack);
}

void rayMarch::RayMarch::update(std::vector<glm::vec3> &feedbackVec)
{
    // should pass by ref

    minCorner = glm::vec3(-0.9);
    maxCorner = glm::vec3(0.9);

    // Compute density texture
    glm::vec3 stepSize =  (maxCorner - minCorner)/ (float)(textureDim-1);

#pragma omp parallel for
    for (int i1=0; i1<texture3D.size(); i1++) {
        if (texture3D.at(i1) >= 0.25) {
            texture3D.at(i1) -= 0.25;
        }
    }

#pragma omp parallel for
    for (int i=0; i<feedbackVec.size()-1; i++) {
        glm::ivec3 index3D;

        index3D = glm::floor(glm::vec3(0.00001) + (feedbackVec.at(i) - minCorner) / stepSize);
        int index = index3D.x + index3D.y*textureDim + index3D.z*textureDim*textureDim;

		if(index >=0 and index<pow(textureDim,3)){
			if (texture3D.at(index) < maxTexVal) {
				texture3D.at(index) += 2.0;
			}
		}
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
    //glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_R32F, textureDim, textureDim, textureDim, 0, GL_RED, GL_FLOAT, texture3D.data());
}

void rayMarch::RayMarch::loadUniforms()
{
}

void rayMarch::RayMarch::genMask(window::Window &w, GLuint backgroundTexture, GLuint backgroundDepth)
{
    glViewport(0, 0, renderWidth, renderHeight);

    // Render to low res texture
    cloud_frame.activate();
    // cloud_frame.clearColour();

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
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, textureDim, textureDim, textureDim, GL_RED, GL_FLOAT, texture3D.data());
    glUniform1i(glGetUniformLocation(mRenderShader->mProgram, "texture3D"), 0); // Assign 3d texture to texture unit 0

    glActiveTexture(GL_TEXTURE1); // Texture unit 1
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glUniform1i(glGetUniformLocation(mRenderShader->mProgram, "framebufferColorTexture"), 1);

    glActiveTexture(GL_TEXTURE2); // Texture unit 2
    glBindTexture(GL_TEXTURE_2D, backgroundDepth);
    glUniform1i(glGetUniformLocation(mRenderShader->mProgram, "framebufferDepthTexture"), 2);

    // Set the max and min corners
    mRenderShader->setUniformVec("minPos", minCorner);
    mRenderShader->setUniformVec("maxPos", maxCorner);
    float fTextDim = (float)textureDim;
    mRenderShader->setUniform("texDim", fTextDim);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);

    glBindVertexArray(0);

    cloud_frame.deactivate();

    glViewport(0, 0, w.width, w.height);
}

void rayMarch::RayMarch::draw(window::Window &w, GLuint backgroundTexture, GLuint backgroundDepth)
{
	w.makeContextCurrent();

    genMask(w, backgroundTexture, backgroundDepth);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    mPostProcessShader->activate();

    glBindVertexArray(VAO);

    // Bind quad to render
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);
    
    //  Draw 1 quad (4 vertices) for every position
    glVertexAttribDivisor(0, 0);

    // Bind and populate Textures
    glActiveTexture(GL_TEXTURE3); // Texture unit 3
    glBindTexture(GL_TEXTURE_2D, backgroundTexture);
    glUniform1i(glGetUniformLocation(mPostProcessShader->mProgram, "backgroundTexture"), 3);

    glActiveTexture(GL_TEXTURE4); // Texture unit 4
    glBindTexture(GL_TEXTURE_2D, cloud_texture.getRef());
    glUniform1i(glGetUniformLocation(mPostProcessShader->mProgram, "cloudTexture"), 4);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);

    glBindVertexArray(0);
}
