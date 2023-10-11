#pragma once

#include "libs/glm/glm.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"

#include "camera.h"
#include "mesh.h"
#include "shader.h"
#include "game_object.h"
//#include "agent.h"
//#include "converters.h"

class Editor
{
private:

    Mesh &mesh;
    Camera &camera;

    glm::mat4 modelMatrix;
public:
    Editor(Mesh &mesh, Camera &camera);
    void update();
    void render(Shader &shader, Shader &fontShader, GameObject* selectedObjectPtr);    
    Mesh* getMesh();
};