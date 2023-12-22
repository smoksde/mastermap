#include "button.h"

#include <iostream>

Button::Button(float x, float y, float width, float height, Mesh &mesh, Mesh &iconMesh, Camera &camera, int item)
    : x(x), y(y), width(width), height(height), mesh(mesh), iconMesh(iconMesh), camera(camera), item(item) 
{
    translationVector = glm::vec3(x, y, 0.0f);
    scalingVector = glm::vec3(width, height, 1.0f);
    updateModelMatrix();
}

void Button::render(Shader &shader)
{
    int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
    int modelViewProjMatrixLocation = glGetUniformLocation(shader.getShaderId(), "u_modelViewProj");

    glUniform4f(colorUniformLocation, 0.1f, 0.1f, 0.1f, 1.0f);

    glm::mat4 modelViewProjMatrix = camera.getViewProj() * getModelMatrix();

    glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);

    shader.bind();

    getMesh()->bind();
    glDrawElements(GL_TRIANGLES, getMesh()->getNumIndices(), GL_UNSIGNED_INT, 0);
    getMesh()->unbind();


    // iconMesh rendering
    colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
    modelViewProjMatrixLocation = glGetUniformLocation(shader.getShaderId(), "u_modelViewProj");

    glUniform4f(colorUniformLocation, 0.9f, 0.9f, 0.9f, 1.0f);

    // specific model matrix for icon
    glm::mat4 iconModelMatrix = glm::mat4(1.0f);
    iconModelMatrix = glm::translate(iconModelMatrix, translationVector);
    iconModelMatrix = glm::scale(iconModelMatrix, glm::vec3(0.6f, 0.6f, 1.0f));

    modelViewProjMatrix = camera.getViewProj() * iconModelMatrix;

    glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);

    shader.bind();

    getIconMesh()->bind();
    glDrawElements(GL_TRIANGLES, getIconMesh()->getNumIndices(), GL_UNSIGNED_INT, 0);
    getIconMesh()->unbind();
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

Mesh *Button::getIconMesh()
{
    return &iconMesh;
}

float Button::getX()
{
    return x;
}

float Button::getY()
{
    return y;
}

float Button::getWidth()
{
    return width;
}

float Button::getHeight()
{
    return height;
}

float Button::getItem()
{
    return item;
}