#pragma once

#include "libs/glm/glm.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"
#include "libs/glm/gtx/string_cast.hpp"

class Camera
{

private:
    glm::mat4 projection;
    glm::mat4 view;
    glm::mat4 viewProj;

    glm::vec3 position;

public:
    Camera(float width, float height)
    {
        // projection = glm::perspective(fov/2.0f, width / height, 0.1f, 1000.0f);
        projection = glm::ortho(-1.0f * width / 2.0f, 1.0f * width / 2.0f, -1.0f * height / 2.0f, 1.0f * height / 2.0f, -1.0f, 1000.0f);
        view = glm::mat4(1.0f);
        position = glm::vec3(0.0f, 0.0f, -8.0f);
        update();
    }

    glm::mat4 getViewProj()
    {
        return viewProj;
    }

    glm::vec3 getPosition()
    {
        return position;
    }

    void update()
    {
        viewProj = projection * glm::translate(view, position);
    }

    void translate(glm::vec3 v)
    {
        position += v * -1.0f;
    }
};