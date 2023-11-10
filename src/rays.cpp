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

    genFBO();
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(g_vertex_buffer_data), g_vertex_buffer_data, GL_STATIC_DRAW);
    glBindVertexArray(0);

    texture3D.insert(texture3D.begin(), flattenedDim, 0.0);

    fillBuffers();
}

rayMarch::RayMarch::RayMarch(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<simulation::Position> startPos, int isTrack)
{
    // init(compShader, renderShader, startPos, isTrack);
}

void rayMarch::RayMarch::genFBO()
{
    glGenFramebuffers(1, &FBO);
    glGenTextures(1, &cloudTex);

    // Setup FBO
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Setup texture
    glBindTexture(GL_TEXTURE_2D, cloudTex);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, renderWidth, renderHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, cloudTex, 0);

    // Check configuration
    GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
	    std::cout << "ERROR::FRAMEBUFFER:: Framebuffer is not complete!" << std::endl;
        switch (status) {
            case GL_FRAMEBUFFER_UNDEFINED:
                printf("GL_FRAMEBUFFER_UNDEFINED\n");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT:
                printf("GL_FRAMEBUFFER_INCOMPLETE_ATTACHMENT\n");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT:
                printf("GL_FRAMEBUFFER_INCOMPLETE_MISSING_ATTACHMENT\n");
                break;
            case GL_FRAMEBUFFER_UNSUPPORTED:
                printf("GL_FRAMEBUFFER_UNSUPPORTED\n");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFER:
                printf("GL_FRAMEBUFFER_INCOMPLETE_DRAW_BUFFE\n");
                break;
            case GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER:
                printf("GL_FRAMEBUFFER_INCOMPLETE_READ_BUFFER\n");
                break;
            // Handle other possible status codes as needed
            default:
                printf("Unknown Error\n");
                break;
        }
    }

    glBindFramebuffer(GL_FRAMEBUFFER, 0); 
}

void rayMarch::RayMarch::update(std::vector<glm::vec3> &feedbackVec)
{
    // should pass by ref

    // printf("%f %f %f\n", feedbackVec[0][0], feedbackVec[0][1], feedbackVec[0][2]);


    minCorner = glm::vec3(-0.9);
    maxCorner = glm::vec3(0.9);

    // Find bounding box
    //for (int i=0; i<feedbackVec.size()-1; i++) {
    //    int subcount = 0;
    //    for (int j=0; j<3; j++) {
    //        if (feedbackVec.at(i)[j] < minCorner[j] && feedbackVec.at(i)[j]!= 0) {
    //            minCorner[j] = feedbackVec.at(i)[j];
    //        }
    //        if (feedbackVec.at(i)[j] > maxCorner[j] && feedbackVec.at(i)[j]!= 0) {
    //            maxCorner[j] = feedbackVec.at(i)[j];
    //        }
    //    }
    //}

    // Compute density texture
    glm::vec3 stepSize =  (maxCorner - minCorner)/ (float)(textureDim-1);
    // printf("Min %f, %f, %f \n", minCorner[0], minCorner[1], minCorner[2]);
    // printf("Max %f, %f, %f \n", maxCorner[0], maxCorner[1], maxCorner[2]);

    texture3D.clear();
    texture3D.insert(texture3D.begin(), flattenedDim, 0.0); //reset the texture to empty

    //for (int i1=0; i1<textureDim; i1++) {
    //    for (int i2=0; i2<textureDim; i2++) {
    //        for (int i3=0; i3<textureDim; i3++) {
    //            if (texture3D.at(i1 + i2*textureDim + i3*textureDim*textureDim) >= 0.05) {
    //                texture3D.at(i1 + i2*textureDim + i3*textureDim*textureDim) -= 0.05;
    //            }
    //        }
    //    }
    //}

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
    glTexSubImage3D(GL_TEXTURE_3D, 0, 0, 0, 0, textureDim, textureDim, textureDim, GL_RED, GL_FLOAT, texture3D.data());
    glUniform1i(glGetUniformLocation(mRenderShader->mProgram, "texture3D"), 0);
    glGetError();

    // Set the max and min corners
    mRenderShader->setUniformVec("minPos", minCorner);
    mRenderShader->setUniformVec("maxPos", maxCorner);
    float fTextDim = (float)textureDim;
    mRenderShader->setUniform("texDim", fTextDim);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);

    glBindVertexArray(0);
}

void rayMarch::RayMarch::genMask(window::Window *w, GLuint backgroundTexture, GLuint backgroundDepth)
{
    glfwMakeContextCurrent(w->getContext());
    glViewport(0, 0, renderWidth, renderHeight);

    // Render to low res texture
    // glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_COLOR_BUFFER_BIT);

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

    // glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // glViewport(0, 0, w->width, w->height);
}

void rayMarch::RayMarch::draw(window::Window *w, GLuint backgroundTexture, GLuint backgroundDepth)
{
    genMask(w, backgroundTexture, backgroundDepth);

    glfwMakeContextCurrent(w->getContext());

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
    glBindTexture(GL_TEXTURE_2D, cloudTex);
    glUniform1i(glGetUniformLocation(mPostProcessShader->mProgram, "cloudTexture"), 4);

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);

    glBindVertexArray(0);
}
