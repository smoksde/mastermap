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
    float zoom;
    float width;
    float height;

public:
    Camera(float nwidth, float nheight)
    {
        // projection = glm::perspective(fov/2.0f, width / height, 0.1f, 1000.0f);
        width = nwidth;
        height = nheight;
        zoom = 1.0f;
        projection = glm::ortho(-zoom * width / 2.0f, zoom * width / 2.0f, -zoom * height / 2.0f, zoom * height / 2.0f, -1.0f, 1000.0f);
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

    void changeZoom(float change)
    {
        zoom += change;
        if (zoom > 8.0f) zoom = 8.0f;
        if (zoom < 1.0f) zoom = 1.0f;
        updateProjection();
    }

    float getZoom()
    {
        return zoom;
    }

    void updateProjection()
    {
        projection = glm::ortho(-zoom * width / 2.0f, zoom * width / 2.0f, -zoom * height / 2.0f, zoom * height / 2.0f, -1.0f, 1000.0f);
        update();
    }
};