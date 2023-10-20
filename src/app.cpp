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

    glfwMakeContextCurrent(w.getContext());
    glfwSetCursorPosCallback(w.getContext(), cursorPosCallback);
    glfwSetInputMode(w.getContext(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    glfwSetKeyCallback(w.getContext(), keyCallback);

    fancyShader.init(fancyShaderFile);
    //fancyShader.init(statcVert);
    quadShader.init(quadVert, quadFrag);
    rayShader.init(rayMarchVert, rayMarchFrag);

    simulation::Position origin(0.0, 0.0, 0.0);//utils::genRandomPoints(1).at(0);
    track::Track initial_track(origin);
    
    std::vector<simulation::Position> track_verts = initial_track.get_vertices();

    track_sim.init(&fancyShader, &rayShader, track_verts, 1);
    //sim.init(&fancyShader, &rayShader, track_verts);
    //ray.init(&fancyShader, &rayShader, track_verts);
}

void app::App::mainLoop()
{

    //sim.update(&w);
    track_sim.update(&w);

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
    glm::mat4 viewMat = glm::inverse(cam.getViewMat());
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), aspectRatio, 0.1f, 100.0f);

    time += 0.001;
    track_sim.mCompShader->setUniform("time", time);
    track_sim.mRenderShader->setUniformVec("view", viewMat);
    track_sim.mRenderShader->setUniformVec("projection", projection);
    // Need to do this with a callback
    track_sim.mRenderShader->setUniform("aspect", aspectRatio);
    // track_sim.mRenderShader->setUniformVec("position", cam.camPos);

    glm::vec3 lightPos = glm::vec3(2.0, 0.0, 0.0);
    glm::vec3 lightColour = glm::vec3(1.0, 1.0, 1.0);
    track_sim.mRenderShader->setUniformVec("lightPos", lightPos);
    track_sim.mRenderShader->setUniformVec("lightColour", lightColour);
    track_sim.mRenderShader->setUniform("time", time);

    //sim.draw(&w);
    track_sim.draw(&w);
    //ray.draw(&w);

    glfwSwapBuffers(w.getContext());

    if(drawFPS % 10 == 0)
    {
        now = emscripten_performance_now() / 1000;
        EM_ASM(document.getElementById("FPSVal").innerHTML = $0;, (int)(10/(now-t)));
        t = now;
    }
    drawFPS++;
}
