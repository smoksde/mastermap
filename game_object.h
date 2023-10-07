#pragma once

#include <cmath>

#include "libs/glm/glm.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"

#include "shader.h"
#include "mesh.h"

#define FACING_UP 0
#define FACING_LEFT 1
#define FACING_DOWN 2
#define FACING_RIGHT 3

#define ROTATION_UP 0.0f
#define ROTATION_LEFT 90.0f
#define ROTATION_DOWN 180.0f
#define ROTATION_RIGHT 270.0f

#define DEFAULT_SCALING_VECTOR glm::vec3(1.0f, 1.0f, 1.0f)
#define DEFAULT_ROTATION_VECTOR glm::vec3(0.0f, 0.0f, 1.0f)

class GameObject
{
private:

    int x;
    int y;
    int z;

    int facing;
    float rotation;

    glm::vec3 translationVector;
    glm::vec3 scalingVector;

    glm::mat4 modelMatrix;

    Mesh mesh;

    bool selected = false;

public:
    GameObject(int x, int y, int z, Mesh &mesh);
    virtual void update();
    void render(Shader &shader);
    Mesh* getMesh();
    void updateModelMatrix();
    glm::mat4 getModelMatrix();
    bool isSelected();
    int getX();
    int getY();
    int getZ();
    void setX(int newX);
    void setY(int newY);
    void setZ(int newZ);
    void updatePosition();
    glm::vec3 getScalingVector();
    void setScalingVector(glm::vec3 scaling);
    float getRotation();
    void setRotation(float rotation);
    int getFacing();
    void setFacing(int facing);

};