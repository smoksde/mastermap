#pragma once

#include "libs/glm/glm.hpp"

// This function converts coordinates from in example the mouse that are pixel based into
// coordinates that are with respect to the OpenGL coordinates system without regarding some
// translation of the camera. It should be used for UI elements that are rendered with the UI camera.
inline glm::vec2 SDL_to_OPENGL(glm::vec2 pos, float width_SDL, float height_SDL, float width_GL, float height_GL)
{
    float input_x = pos[0];
    float input_y = pos[1];

    input_x = input_x - (width_SDL / 2.0f);
    input_y = input_y - (height_SDL / 2.0f);

    input_x = input_x * ((width_GL / 2.0f) / (width_SDL / 2.0f));
    input_y = input_y * ((height_GL / 2.0f) / (height_SDL / 2.0f));

    return glm::vec2(input_x, -input_y);
}

// Converts coordinates from the OpenGL coordinates system that are between [-1,1] to the
// pixel based SDL coordinates
inline glm::vec2 OPENGL_to_SDL(glm::vec2 pos, float width_SDL, float height_SDL, float width_GL, float height_GL)
{
    float input_x = pos[0];
    float input_y = -pos[1];

    input_x = input_x / ((width_GL / 2.0f) / (width_SDL / 2.0f));
    input_y = input_y / ((height_GL / 2.0f) / (height_SDL / 2.0f));

    input_x = input_x + (width_SDL / 2.0f);
    input_y = input_y + (height_SDL / 2.0f);

    return glm::vec2(input_x, input_y);
}

inline glm::vec2 INGAME_to_SDL_wrt_CAM(glm::vec2 pos, float width_SDL, float height_SDL, float width_INGAME, float height_INGAME, glm::vec2 camPos, float zoom)
{
    float input_x = pos[0];
    float input_y = pos[1];

    // INGAME to GL part

    //input_x = input_x - camPos[0];
    //input_y = input_y - camPos[1];

    //input_x = -input_x;
    //input_y = -input_y;

    //input_x = input_x / width_INGAME * zoom;
    //input_y = input_y / height_INGAME * zoom;

    // GL to SDL part

    input_x = -input_x / ((width_INGAME / 2.0f)* zoom / (width_SDL / 2.0f));
    input_y = input_y / ((height_INGAME / 2.0f)* zoom / (height_SDL / 2.0f));

    input_x = input_x + (width_SDL / 2.0f);
    input_y = input_y + (height_SDL / 2.0f);

    return glm::vec2(input_x, input_y);
}