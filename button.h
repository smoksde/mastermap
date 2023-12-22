#pragma once

#include "game_object.h"

#include "libs/glm/glm.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"

class Button
{
private:
    float x;
    float y;
    
    float width;
    float height;

    glm::vec3 translationVector;
    glm::vec3 scalingVector;
    glm::mat4 modelMatrix;

    Mesh &mesh;
    Mesh &iconMesh;
    Camera &camera;

    int item;
public:
    Button(float x, float y, float width, float height, Mesh &mesh, Mesh &iconMesh, Camera &camera, int item);
    void render(Shader &shader);
    void updateModelMatrix();
    glm::mat4 getModelMatrix();
    Mesh* getMesh();
    Mesh* getIconMesh();
    float getX();
    float getY();
    float getWidth();
    float getHeight();
    float getItem();
};