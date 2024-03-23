#pragma once

#include <cmath>
#include <memory>
#include <list>

#include "libs/glm/glm.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"

#include "shader.h"
#include "mesh.h"
#include "camera.h"

#define FACING_UP 0
#define FACING_LEFT 1
#define FACING_DOWN 2
#define FACING_RIGHT 3

#define ROTATION_UP 0.0f
#define ROTATION_LEFT 90.0f
#define ROTATION_DOWN 180.0f
#define ROTATION_RIGHT 270.0f

// #define DEFAULT_SCALING_VECTOR glm::vec3(1.0f, 1.0f, 1.0f)
#define DEFAULT_SCALING_VECTOR glm::vec3(0.8f, 0.8f, 0.8f)
#define DEFAULT_ROTATION_VECTOR glm::vec3(0.0f, 0.0f, 1.0f)

class GameObject
{
private:

    int x;
    int y;
    int z;

    GameObject &parent;

    int facing = FACING_UP;
    float rotation = ROTATION_UP;

    float preRotation = ROTATION_UP;
    float renderRotation = ROTATION_UP;

    glm::vec3 translationVector;
    glm::vec3 scalingVector;

    glm::mat4 modelMatrix;

    Mesh &mesh;
    bool selected = false;

protected:
    RGBAColor color;
    
public:

    int preX;
    int preY;

    Camera &camera;
    std::list<std::unique_ptr<GameObject>> &objects;

    GameObject(int x, int y, int z, Mesh &mesh, Camera &camera, RGBAColor color, std::list<std::unique_ptr<GameObject>> &objects, GameObject &parent);
    virtual void tick();
    virtual void update(float elapsUpdate);
    virtual void render(Shader &shader);
    void rotate();
    Mesh* getMesh();
    void updateModelMatrix();
    glm::mat4 getModelMatrix();
    bool isSelected();
    void select();
    void deselect();
    int getX();
    int getY();
    int getZ();
    void setX(int newX);
    void setY(int newY);
    void setZ(int newZ);
    void updatePosition();
    glm::vec3 getTranslationVector();
    glm::vec3 getScalingVector();
    void setScalingVector(glm::vec3 scaling);
    float getRotation();
    void setRotation(float rotation);
    int getFacing();
    void setFacing(int facing);
    glm::vec4 getColor();
    void setColor(glm::vec4 values);
};