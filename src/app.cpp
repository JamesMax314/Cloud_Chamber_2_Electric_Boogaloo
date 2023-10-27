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

std::vector<glm::vec3> boxVerts {
        glm::vec3(-1, -1,  1), //0
        glm::vec3( 1, -1,  1), //1
        glm::vec3(-1,  1,  1), //2
        glm::vec3( 1,  1,  1), //3
        glm::vec3(-1, -1, -1), //4
        glm::vec3( 1, -1, -1), //5
        glm::vec3(-1,  1, -1), //6
        glm::vec3( 1,  1, -1)  //7
    };

std::vector<GLuint> boxInds {
        //Top
        2, 6, 7,
        2, 3, 7,

        //Bottom
        0, 4, 5,
        0, 1, 5,

        //Left
        0, 2, 6,
        0, 4, 6,

        //Right
        1, 3, 7,
        1, 5, 7,

        //Front
        0, 2, 3,
        0, 1, 3,

        //Back
        4, 6, 7,
        4, 5, 7
    };

app::App::App()
{
}

bool keys[GLFW_KEY_LAST] = { false };

int lr = 0;
int ud = 0;
int fb = 0;

double prevMouseX = 0.0;
double prevMouseY = 0.0;
double deltaX;
double deltaY;

int forward = 0;

// Mouse cursor position callback function
void cursorPosCallback(GLFWwindow* window, double xpos, double ypos) {
    // Calculate the change in mouse cursor position
    deltaX = xpos - prevMouseX;
    deltaY = ypos - prevMouseY;

    // Update the previous mouse cursor position
    prevMouseX = xpos;
    prevMouseY = ypos;

    // Use deltaX and deltaY to respond to mouse movement
    // Your code here
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods) {
    if (action == GLFW_PRESS) {
        keys[key] = true;
        // printf("Keys: %i\n", key);
        forward = 1;
        if (key == GLFW_KEY_W) {
            fb = 1;
        }
        if (key == GLFW_KEY_S) {
            fb = -1;
        }
        if (key == GLFW_KEY_A) {
            lr = 1;
        }
        if (key == GLFW_KEY_D) {
            lr = -1;
        }
        if (key == GLFW_KEY_SPACE) {
            ud = 1;
        }
        if (key == GLFW_KEY_LEFT_SHIFT) {
            ud = -1;
        }

    } else if (action == GLFW_RELEASE) {
        if (key == GLFW_KEY_W) {
            fb = 0;
        }
        if (key == GLFW_KEY_S) {
            fb = 0;
        }
        if (key == GLFW_KEY_A) {
            lr = 0;
        }
        if (key == GLFW_KEY_D) {
            lr = 0;
        }
        if (key == GLFW_KEY_SPACE) {
            ud = 0;
        }
        if (key == GLFW_KEY_LEFT_SHIFT) {
            ud = 0;
        }
    }
}



void app::App::init()
{
    const char* vertexShaderFile = "../shaders/triangleVert.glsl";
    const char* fragmentShaderFile = "../shaders/triangleFrag.glsl";

    const char* sqrtShaderFile = "../shaders/sqrtVert.glsl";
    const char* fancyShaderFile = "../shaders/fancyVert.glsl";
    const char* statcVert = "../shaders/staticVert.glsl";

    const char* quadFrag = "../shaders/quadFrag.glsl";
    const char* quadVert = "../shaders/quadVert.glsl";

    const char* rayMarchFrag = "../shaders/rayMarchFrag.glsl";
    const char* rayMarchVert = "../shaders/rayMarchVert.glsl";


    const char* basicVert = "../shaders/basicVert.glsl";
    const char* basicFrag = "../shaders/basicFrag.glsl";

    const char* vertexPostProcess = "../shaders/postProcessVert.glsl";
    const char* fragmentPostProcess = "../shaders/postProcessFrag.glsl";

    const char* vertexTexture = "../shaders/textureVert.glsl";
    const char* fragmentTexture = "../shaders/textureFrag.glsl";


    glfwMakeContextCurrent(w.getContext());
    glfwSetCursorPosCallback(w.getContext(), cursorPosCallback);
    glfwSetInputMode(w.getContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(w.getContext(), keyCallback);

    fancyShader.init(fancyShaderFile);
    //fancyShader.init(statcVert);
    quadShader.init(quadVert, quadFrag);
    rayShader.init(rayMarchVert, rayMarchFrag);
    basicShader.init(basicVert, basicFrag);
    postProcessShader.init(vertexPostProcess, fragmentPostProcess);
    densityCompShader.init(vertexTexture, fragmentTexture);

    std::vector<simulation::Position> origin = utils::genRandomPoints(2);
    
    std::vector<track::Track> tracks;
    std::vector<simulation::Position> track_verts;

    for(int i =0; i<2; i++){
	tracks.emplace_back(origin.at(i));
	std::vector<glm::vec3> temp_track_verts = tracks.at(i).get_vertices();
	track_verts.insert(track_verts.end(), temp_track_verts.begin(), temp_track_verts.end());
    }

    ray_marcher.init(&fancyShader, &rayShader, &postProcessShader);
    track_sim.init(&fancyShader, &quadShader, track_verts, 1);

    std::vector<simulation::Position> bg_verts = utils::genRandomPoints(10000);
    sim.init(&fancyShader, &quadShader, bg_verts, 0);

    boundingBox.init(&basicShader, boxVerts, boxInds);
    boundingBox.drawType = GL_LINES;

    bubbleColourTex.initColour(w.width, w.height);
    bubbleDepthTex.initDepth(w.width, w.height);
    frameBufferBackBubbles.init(&bubbleColourTex, &bubbleDepthTex);

    densitySim.init(&densityCompShader);

    glEnable(GL_DEPTH_TEST);
}

void app::App::mainLoop()
{

    //Generate new track
    
    double p = uniform_dist(rand_gen);
    densitySim.update(&w);
 
    if(p < 0.004){
	std::cout<<"Track created"<<std::endl;
	std::vector<simulation::Position> origin = utils::genRandomPoints(1);
	track::Track new_track(glm::vec3(0.0));
    	std::vector<glm::vec3> temp_track_verts = new_track.get_vertices();
	track_sim.addVerts(temp_track_verts);
    }

    sim.update(&w);
    track_sim.update(&w);

    glClear(GL_COLOR_BUFFER_BIT);
    frameBufferBackBubbles.clear();

    float dt = 1;

    // Move camera
    if (fb != 0) {
        cam.move(0.0f, 0.0f, fb*motionSpeed*dt);
    }
    if (lr != 0) {
        cam.move(lr*motionSpeed*dt, 0.0f, 0.0f);
    }
    if (ud != 0) {
        cam.move(0.0f, -ud*motionSpeed*dt, 0.0f);
    }

    // Rotate Camera
    if (deltaX != 0.0 || deltaY != 0.0) {
        cam.rotate(deltaX, deltaY);
        deltaX = 0;
        deltaY = 0;
    }

    // Compute the current view and perspective matrices
    float aspectRatio = w.getAspect();
    glm::mat4 viewMat = cam.getViewMat();
    glm::mat4 inverseViewMat = glm::inverse(cam.getViewMat());
    float nearClip = 0.01f;
    float farClip = 5.0f;
    float fovRad = glm::radians(45.0f);
    glm::mat4 projection = glm::perspective(fovRad, aspectRatio, nearClip, farClip);

    time += 0.001;
    sim.mCompShader->setUniform("time", time);
    track_sim.mCompShader->setUniform("time", time);
    ray_marcher.mCompShader->setUniform("time", time);
    ray_marcher.mRenderShader->setUniformVec("view", viewMat);
    sim.mRenderShader->setUniformVec("view", viewMat);
    track_sim.mRenderShader->setUniformVec("view", viewMat);
    ray_marcher.mRenderShader->setUniformVec("projection", projection);
    sim.mRenderShader->setUniformVec("projection", projection);
    track_sim.mRenderShader->setUniformVec("projection", projection);
    // Need to do this with a callback
    ray_marcher.mRenderShader->setUniform("aspect", aspectRatio);
    ray_marcher.mRenderShader->setUniform("nearClip", nearClip);
    ray_marcher.mRenderShader->setUniform("farClip", farClip);
    ray_marcher.mRenderShader->setUniform("fovRad", fovRad);

    basicShader.setUniformVec("view", viewMat);
    basicShader.setUniformVec("projection", projection);

    glm::vec3 lightPos = glm::vec3(2.0, 0.0, 0.0);
    glm::vec3 lightColour = glm::vec3(1.0, 1.0, 1.0);
    ray_marcher.mRenderShader->setUniformVec("lightPos", lightPos);
    ray_marcher.mRenderShader->setUniformVec("lightColour", lightColour);

    frameBufferBackBubbles.activate();
    sim.draw(&w);
    //track_sim.draw(&w); //Draw track particles
    boundingBox.draw(w.getContext());
    frameBufferBackBubbles.deactivate();

    // ray_marcher.draw(&w);
    //ray.draw(&w);
    // boundingBox.draw(w.getContext());

    track_sim.updateFeedbackVec();
    ray_marcher.update(track_sim.feedbackVec);
    ray_marcher.draw(&w, bubbleColourTex.m_textureRef, bubbleDepthTex.m_textureRef);

    // ray_marcher.draw(&w);

    glfwSwapBuffers(w.getContext());

    if(drawFPS % 10 == 0)
    {
        now = emscripten_performance_now() / 1000;
        EM_ASM(document.getElementById("FPSVal").innerHTML = $0;, (int)(10/(now-t)));
        t = now;
    }

    GLenum error = glGetError();
    if (error != GL_NO_ERROR) {
        printf("Error %u \n", error);
    }

    drawFPS++;
}
