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
        printf("Keys: %i\n", key);
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

    glfwMakeContextCurrent(w.getContext());
    glfwSetCursorPosCallback(w.getContext(), cursorPosCallback);
    glfwSetInputMode(w.getContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(w.getContext(), keyCallback);

    fancyShader.init(fancyShaderFile);
    //fancyShader.init(statcVert);
    quadShader.init(quadVert, quadFrag);
    rayShader.init(rayMarchVert, rayMarchFrag);
    basicShader.init(basicVert, basicFrag);

    simulation::Position origin(0.0, 0.0, -1.0);//utils::genRandomPoints(1).at(0);
    track::Track initial_track(origin);
    
    std::vector<simulation::Position> track_verts = initial_track.get_vertices();

    track_sim.init(&fancyShader, &rayShader, track_verts, 1);
    sim.init(&fancyShader, &quadShader, track_verts, 1);

    boundingBox.init(&basicShader, boxVerts, boxInds);
    // boundingBox.drawType = GL_LINES;

    glEnable(GL_DEPTH_TEST);
    initBuffers();
    // ray.init(&fancyShader, &rayShader, track_verts);
}

void app::App::initBuffers()
{
    // Initialise frame buffer object
    // Bind FBO
    // Gen texture
    // bind texture
    // glTexImage2D to specify we want a colour and depth buffer texture
    // Specify interpolation
    // Unbind texture
    // glFramebufferTexture2D to bind texture to FBO
    // Check FBO complete 
    // Unbind FBO

    glGenFramebuffers(1, &FBO);
    glGenTextures(1, &textureOut);
    glGenTextures(1, &depthOut);

    // Setup FBO
    glBindFramebuffer(GL_FRAMEBUFFER, FBO);

    // Setup texture
    glBindTexture(GL_TEXTURE_2D, textureOut);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w.width, w.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR );
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);

    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, textureOut, 0);

    // Setup depth buffer
    glBindTexture(GL_TEXTURE_2D, depthOut);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, w.width, w.height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    // Attach the depth texture to the FBO
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, depthOut, 0);
    
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

void app::App::mainLoop()
{

    sim.update(&w);
    // track_sim.update(&w);

    glClear(GL_COLOR_BUFFER_BIT);

    float dt = 1;

    // Move camera
    if (fb != 0) {
        printf("w %i\n", fb);
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
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    time += 0.001;
    track_sim.mCompShader->setUniform("time", time);
    track_sim.mRenderShader->setUniformVec("view", inverseViewMat);
    sim.mRenderShader->setUniformVec("view", viewMat);
    track_sim.mRenderShader->setUniformVec("projection", projection);
    sim.mRenderShader->setUniformVec("projection", projection);
    // Need to do this with a callback
    track_sim.mRenderShader->setUniform("aspect", aspectRatio);

    basicShader.setUniformVec("view", viewMat);
    basicShader.setUniformVec("projection", projection);

    glm::vec3 lightPos = glm::vec3(2.0, 0.0, 0.0);
    glm::vec3 lightColour = glm::vec3(1.0, 1.0, 1.0);
    track_sim.mRenderShader->setUniformVec("lightPos", lightPos);
    track_sim.mRenderShader->setUniformVec("lightColour", lightColour);

    glBindFramebuffer(GL_FRAMEBUFFER, FBO);
    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
    // sim.draw(&w);
    boundingBox.draw(w.getContext());
    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    // track_sim.draw(&w);
    //ray.draw(&w);
    // boundingBox.draw(w.getContext());

    track_sim.draw(&w, textureOut, depthOut);

    // track_sim.draw(&w);

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
