#include "drawables.hpp"

drawable::Drawable::Drawable()
{
}

drawable::Drawable::Drawable(shaders::Shader *shader)
{
    mShader = shader;
}

void drawable::Drawable::fillBuffers()
{
    // Bind all the model arrays to the appropriate buffers
    glBindVertexArray(VAO);

    // Vertex buffer contains vertices and indices interleafed
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, mVertices.size()*sizeof(Vertex), &mVertices.front(), GL_STATIC_DRAW);

    // Tell the shader how the interleafing should be undone
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);

    // Index buffer contains just the indices
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, mIndices.size()*sizeof(unsigned int), &mIndices.front(), GL_STATIC_DRAW);
}

void drawable::Drawable::loadUniforms()
{

}

void drawable::Drawable::draw(GLFWwindow* w)
{
    glfwMakeContextCurrent(w);
    mShader->activate();
    // Bind model buffers and draw to screen
    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glDrawElements(GL_TRIANGLES, mIndices.size(), GL_UNSIGNED_INT, 0);
}