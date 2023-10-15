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

double prevMouseX = 0.0;
double prevMouseY = 0.0;
double deltaX;
double deltaY;

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

    // fancyShader.init(fancyShaderFile);
    fancyShader.init(statcVert);
    quadShader.init(quadVert, quadFrag);
    rayShader.init(rayMarchVert, rayMarchFrag);


    std::vector<simulation::Position> randParticles = utils::genRandomPoints(1);

    // sim.init(&fancyShader, &quadShader, randParticles);
    sim.init(&fancyShader, &rayShader, randParticles);
    ray.init(&fancyShader, &rayShader, randParticles);
}

void app::App::mainLoop()
{

    // sim.update(&w);

    glClear(GL_COLOR_BUFFER_BIT);

    float dt = 1;

    if (keys[GLFW_KEY_W]) {
        printf("w\n");
        cam.move(0.0f, 0.0f, motionSpeed*dt);
    }
    if (keys[GLFW_KEY_S]) {
        cam.move(0.0f, 0.0f, -motionSpeed*dt);
    }
    if (keys[GLFW_KEY_A]) {
        cam.move(motionSpeed*dt, 0.0f, 0.0f);
    }
    if (keys[GLFW_KEY_D]) {
        cam.move(-motionSpeed*dt, 0.0f, 0.0f);
    }
    if (keys[GLFW_KEY_SPACE]) {
        cam.move(0.0f, motionSpeed*dt, 0.0f);
    }
    if (keys[GLFW_KEY_LEFT_SHIFT]) {
        cam.move(0.0f, -motionSpeed*dt, 0.0f);
    }

    if (deltaX != 0.0 || deltaY != 0.0) {
        cam.rotate(deltaX, deltaY);
        deltaX = 0;
        deltaY = 0;
    }


    time += 0.001;
    pos += 0.1;
    // sim.mCompShader->setUniform("time", time);
    // sim.mRenderShader->setUniformVec("view", cam.viewMatrix);
    // Need to do this with a callback
    float aspectRatio = w.getAspect();
    ray.mRenderShader->setUniform("aspect", aspectRatio);
    glm::vec3 position = pos*glm::vec3(0.0, 0.0, 0.1);
    ray.mRenderShader->setUniformVec("position", position);

    // sim.draw(&w);
    ray.draw(&w);

    glfwSwapBuffers(w.getContext());

    if(drawFPS % 10 == 0)
    {
        now = emscripten_performance_now() / 1000;
        EM_ASM(document.getElementById("FPSVal").innerHTML = $0;, (int)(10/(now-t)));
        t = now;
    }
    drawFPS++;
}
