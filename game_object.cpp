#include "game_object.h"
#include <SDL2/SDL.h>

GameObject::GameObject(int x, int y, int z, Mesh &mesh, Camera &camera)
    : x(x), y(y), z(z), translationVector(glm::vec3(float(x), float(y), float(z))), mesh(mesh), camera(camera)
{
    scalingVector = DEFAULT_SCALING_VECTOR;
    rotation = ROTATION_UP;
    facing = FACING_UP;

    updateModelMatrix();
}

void GameObject::update()
{
    
}

void GameObject::render(Shader &shader)
{

    int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
    int modelViewProjMatrixLocation = glGetUniformLocation(shader.getShaderId(), "u_modelViewProj");

    glm::mat4 modelViewProjMatrix = camera.getViewProj() * getModelMatrix();

    glUniform4f(colorUniformLocation, 0.1f, 0.1f, 0.1f, 1.0f);
    glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);

    shader.bind();

    getMesh()->bind();

    // Perform rendering using OpenGL (e.g., glDrawElements)
    glDrawElements(GL_TRIANGLES, getMesh()->getNumIndices(), GL_UNSIGNED_INT, 0);

    // Unbind buffers when done
    getMesh()->unbind();
}

Mesh *GameObject::getMesh()
{
    return &mesh;
}

void GameObject::updateModelMatrix()
{
    modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, translationVector);
    modelMatrix = glm::scale(modelMatrix, scalingVector);
    modelMatrix = glm::rotate(modelMatrix, float(rotation * (M_PI / 180.0f)), DEFAULT_ROTATION_VECTOR);
}

glm::mat4 GameObject::getModelMatrix()
{
    return modelMatrix;
}

bool GameObject::isSelected()
{
    return selected;
}

void GameObject::select()
{
    selected = true;
}

void GameObject::deselect()
{
    selected = false;
}

int GameObject::getX()
{
    return x;
}

int GameObject::getY()
{
    return y;
}

int GameObject::getZ()
{
    return z;
}

void GameObject::setX(int newX)
{
    x = newX;
    updatePosition();
}

void GameObject::setY(int newY)
{
    y = newY;
    updatePosition();
}

void GameObject::setZ(int newZ)
{
    z = newZ;
    updatePosition();
}

void GameObject::updatePosition()
{
    translationVector = glm::vec3(float(getX()), float(getY()), float(getZ()));
    updateModelMatrix();
}

glm::vec3 GameObject::getScalingVector()
{
    return scalingVector;
}

void GameObject::setScalingVector(glm::vec3 scaling)
{
    scalingVector = scaling;
    updateModelMatrix();
}

float GameObject::getRotation()
{
    return rotation;
}

void GameObject::setRotation(float newRotation)
{
    rotation = newRotation;
    updateModelMatrix();
}

int GameObject::getFacing()
{
    return facing;
}

void GameObject::setFacing(int newFacing)
{
    facing = newFacing;
    switch (facing)
    {
    case FACING_UP:
        rotation = ROTATION_UP;
        break;
    case FACING_LEFT:
        rotation = ROTATION_LEFT;
        break;
    case FACING_DOWN:
        rotation = ROTATION_DOWN;
        break;
    case FACING_RIGHT:
        rotation = ROTATION_RIGHT;
        break;
    default:
        rotation = ROTATION_UP;
        break;
    }
    updateModelMatrix();
}