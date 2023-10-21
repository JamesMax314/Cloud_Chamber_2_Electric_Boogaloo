#include "drawables.hpp"

drawable::Drawable::Drawable()
{
}

drawable::Drawable::Drawable(shaders::Shader *shader)
{
    mShader = shader;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
}

drawable::Drawable::Drawable(shaders::Shader *shader, std::vector<glm::vec3> vertices, std::vector<unsigned int> indices)
{
    mShader = shader;
    mVertices = vertices;
    mIndices = indices;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    fillBuffers();
}

void drawable::Drawable::init(shaders::Shader *shader, std::vector<glm::vec3> vertices, std::vector<unsigned int> indices)
{
    mShader = shader;
    mVertices = vertices;
    mIndices = indices;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    fillBuffers();
}

void drawable::Drawable::fillBuffers()
{
    // Bind all the model arrays to the appropriate buffers
    glBindVertexArray(VAO);

    // Vertex buffer contains vertices and indices interleafed
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), &mVertices.front(), GL_STATIC_DRAW);

    // Index buffer contains just the indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(unsigned int), &mIndices.front(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void drawable::Drawable::loadUniforms()
{

}

void drawable::Drawable::draw(GLFWwindow* w)
{
    glfwMakeContextCurrent(w);
    mShader->activate();
    // printf("activated shader\n");

    // Bind model buffers and draw to screen
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glDrawElements(drawType, mIndices.size(), GL_UNSIGNED_INT, 0);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    // glDrawArrays(GL_TRIANGLES, 0, 4);
    // glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}