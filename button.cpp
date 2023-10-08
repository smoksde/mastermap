#include "button.h"

#include <iostream>

Button::Button(float x, float y, float width, float height, Mesh &mesh)
    : x(x), y(y), width(width), height(height), mesh(mesh), translationVector(glm::vec3(x, y, 0.0f))
{
    scalingVector = glm::vec3(width, height, 1.0f);
    updateModelMatrix();
}

void Button::update(float mouseX, float mouseY)
{

    std::cout << "mouseX, mouseY " << mouseX << ", " << mouseY;
    std::cout << "x, y " << x << ", " << y;

    if (std::abs(x - mouseX) <= 0.5f && std::abs(y - mouseY) <= 0.5f)
    {
        std::cout << "in button" << std::endl;
    }
    else
    {
        std::cout << "out button" << std::endl;
    }
}

void Button::render(Shader &shader)
{
    shader.bind();

    getMesh()->bind();
    glDrawElements(GL_TRIANGLES, getMesh()->getNumIndices(), GL_UNSIGNED_INT, 0);
    getMesh()->unbind();
}

void Button::updateModelMatrix()
{
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, translationVector);
    modelMatrix = glm::scale(modelMatrix, scalingVector);
}

glm::mat4 Button::getModelMatrix()
{
    return modelMatrix;
}

Mesh *Button::getMesh()
{
    return &mesh;
}