#include "simulation.hpp"

simulation::Sim::Sim()
{
}

simulation::Sim::~Sim()
{
}

simulation::Sim::Sim(shaders::Shader *shader)
{
}

void simulation::Sim::init(shaders::Shader *compShader, shaders::Shader *renderShader, std::vector<simulation::Position> startPos, int isTrack)
{
    for (int i = 0; i < nVerts; i++){
	current_index = (current_index+i)%nVerts;
	feedbackVec.push_back(simulation::Position(2.0));
    }
    
    this->isTrack = isTrack;
    mCompShader = compShader;
    mRenderShader = renderShader;
    mStartPos = startPos;
    for (int i = 0; i < mStartPos.size(); i++){
	current_index = (current_index+i)%nVerts;
	feedbackVec.at(current_index) = mStartPos.at(i);
    }
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &ParticleBufferA);
    glGenBuffers(1, &ParticleBufferB);
    glGenBuffers(1, &billboard_vertex_buffer);
    fillBuffers();
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
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferA);

    if(newVerts.size() > 0){
	int n_new_Verts = newVerts.size();

    	if(current_index + 1 + n_new_Verts < nVerts){//Check if buffer has enough memory
    	    glBufferSubData(GL_ARRAY_BUFFER, (current_index+1)*sizeof(simulation::Position), n_new_Verts*sizeof(simulation::Position), newVerts.data());
    	}else{
    	    //Split the data into two pieces and start writing the second piece from the beginning of the buffer
    	    int spare_space = (nVerts-1)-current_index;
    	    glBufferSubData(GL_ARRAY_BUFFER, (current_index+1)*sizeof(simulation::Position), spare_space*sizeof(simulation::Position), newVerts.data());
    	    glBufferSubData(GL_ARRAY_BUFFER, 0, (n_new_Verts-spare_space)*sizeof(simulation::Position), &newVerts.at(spare_space-1));
    	}
	newVerts.clear();

    	current_index = (current_index + n_new_Verts)%nVerts;
    }

    // Turn off drawing
    glEnable(GL_RASTERIZER_DISCARD);

    // Specify the target buffer:
    // Specify input format
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Set output buffer
    glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, 0, ParticleBufferB);
    glBindBuffer(GL_ARRAY_BUFFER, 0);

    // Begin Reading data 
    glBeginTransformFeedback(GL_POINTS);

    // Do Calculation
    glDrawArrays(GL_POINTS, 0, nVerts);

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
    glBufferData(GL_ARRAY_BUFFER, nVerts*sizeof(simulation::Position), &feedbackVec.front(), GL_STREAM_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Create VBO for output on even-numbered frames and input on odd-numbered frames:
    glBindBuffer(GL_ARRAY_BUFFER, ParticleBufferB);
    glBufferData(GL_ARRAY_BUFFER, nVerts*sizeof(simulation::Position), (void*)0, GL_STREAM_DRAW);

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
    glGetBufferSubData(GL_ARRAY_BUFFER, 0, feedbackVec.size()*sizeof(glm::vec3), feedbackVec.data());
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

    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 36, nVerts);

    glBindVertexArray(0);
}

void simulation::Sim::addVerts(std::vector<simulation::Position>& new_verts){
    
    for (int i = 0; i < new_verts.size(); i++){
	newVerts.push_back(new_verts.at(i));
    }
    std::cout<<newVerts.size()<<std::endl;
}

simulation::DensitySim::DensitySim()
{
}

simulation::DensitySim::~DensitySim()
{
    delete flattenedCloudTexIn;
    delete flattenedCloudTexOut;
}

void simulation::DensitySim::init(shaders::Shader *compShader, shaders::Shader *trackShader)
{
    // Initialise a colour texture to render the volume to
    flattenedCloudTexIn = new texture::Texture();
    flattenedCloudTexOut = new texture::Texture();
    flattenedCloudTexIn->initColour(cloudTexDim, cloudTexDim);
    // flattenedCloudTexIn->setColour(glm::vec4(1.0));
    flattenedCloudTexOut->initColour(cloudTexDim, cloudTexDim);
    frameBufferCloudDen.init(flattenedCloudTexOut);

    mCompShader = compShader;
    mTrackShader = trackShader;

    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &billboard_vertex_buffer);
    
    glBindVertexArray(VAO);

    // Bind rendering buffers
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(zeroToOneQuad), zeroToOneQuad, GL_STATIC_DRAW);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void simulation::DensitySim::update(window::Window* w)
{
    // Make context current
    glfwMakeContextCurrent(w->getContext());

    // Bind FBO
    frameBufferCloudDen.activate();
    frameBufferCloudDen.clear();

    // Bind VAO
    glBindVertexArray(VAO);

    // Activate compute shader
    mCompShader->activate();

    // Set window dimensions
    glViewport(0, 0, cloudTexDim, cloudTexDim);

    // Bind render quad 
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Bind cloud texture in to read from
    glActiveTexture(GL_TEXTURE0); // Texture unit 0
    glBindTexture(GL_TEXTURE_2D, flattenedCloudTexIn->getRef());
    glUniform1i(glGetUniformLocation(mCompShader->mProgram, "texture2D"), 0);

    printf("Setup draw\n");

    // Draw call
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);

    printf("Draw complete \n");

    // Deactivate VAO
    glBindVertexArray(0);

    // Swap reading and wirthin textures
    std::swap(flattenedCloudTexIn, flattenedCloudTexOut);
    frameBufferCloudDen.setRenderTexture(flattenedCloudTexOut);
    printf("Swapped \n");

    // Reset window dimensions
    glViewport(0, 0, w->width, w->height);

    // Deactivate FBO
    frameBufferCloudDen.deactivate();





        // Bind VAO
    glBindVertexArray(VAO);

    // Activate compute shader
    mCompShader->activate();

    // Set window dimensions
    glViewport(0, 0, cloudTexDim, cloudTexDim);

    // Bind render quad 
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Bind cloud texture in to read from
    glActiveTexture(GL_TEXTURE0); // Texture unit 0
    glBindTexture(GL_TEXTURE_2D, flattenedCloudTexIn->getRef());
    glUniform1i(glGetUniformLocation(mCompShader->mProgram, "texture2D"), 0);

    // Draw call
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);

    // Deactivate VAO
    glBindVertexArray(0);
    
    // glfwSwapBuffers(w->getContext());
}

void simulation::DensitySim::addTrack(window::Window* w, std::vector<glm::vec3> &positions)
{
    // Make context current
    glfwMakeContextCurrent(w->getContext());

    // Bind FBO
    frameBufferCloudDen.activate();
    frameBufferCloudDen.clear();

    // Bind VAO
    glBindVertexArray(VAO);

    // Activate compute shader
    mTrackShader->activate();
    mTrackShader->setUniformArrVec("trackPoints", positions.data(), positions.size());
    int trackID = 0;
    mTrackShader->setUniform("trackID", trackID);

    // Set positions uniform

    // Set window dimensions
    glViewport(0, 0, cloudTexDim, cloudTexDim);

    // Bind render quad 
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, billboard_vertex_buffer);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*)0);

    // Bind cloud texture in to read from
    glActiveTexture(GL_TEXTURE0); // Texture unit 0
    glBindTexture(GL_TEXTURE_2D, flattenedCloudTexIn->getRef());
    glUniform1i(glGetUniformLocation(mTrackShader->mProgram, "texture2D"), 0);

    // Draw call
    glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, 1);

    // Deactivate VAO
    glBindVertexArray(0);

    // Swap reading and wirthin textures
    std::swap(flattenedCloudTexIn, flattenedCloudTexOut);
    frameBufferCloudDen.setRenderTexture(flattenedCloudTexOut);

    // Reset window dimensions
    glViewport(0, 0, w->width, w->height);

    // Deactivate FBO
    frameBufferCloudDen.deactivate();

    printf("Track Added\n");
}
