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

void app::App::init()
{
    const char* vertexShaderFile = "../shaders/triangleVert.glsl";
    const char* fragmentShaderFile = "../shaders/triangleFrag.glsl";

    const char* sqrtShaderFile = "../shaders/sqrtVert.glsl";
    const char* fancyShaderFile = "../shaders/fancyVert.glsl";

    const char* quadFrag = "../shaders/quadFrag.glsl";
    const char* quadVert = "../shaders/quadVert.glsl";

    glfwMakeContextCurrent(w.getContext());

    fancyShader.init(fancyShaderFile);
    quadShader.init(quadVert, quadFrag);

    std::vector<simulation::Position> randParticles = utils::genRandomPoints(10);
    printf("RandPos %f %f %f \n", randParticles[0][0], randParticles[0][1], randParticles[0][2]);

    std::vector<simulation::Position> sParticles(1, drawable::Vertex(0, 0, 0));
    sim.init(&fancyShader, &quadShader, randParticles);
}

void app::App::mainLoop()
{

    sim.update(&w);

    glClear(GL_COLOR_BUFFER_BIT);

    sim.draw(&w);

    glfwSwapBuffers(w.getContext());
}
