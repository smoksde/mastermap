#include <iostream>
#include <cmath>
#include <list>
#define GLEW_STATIC
#include <GL/glew.h>
#define SDL_MAIN_HANDLED

#define STB_IMAGE_IMPLEMENTATION
#include "libs/stb_image.h"

#include "libs/glm/glm.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"

#ifdef _WIN32
#include <SDL.h>
#pragma comment(lib, "SDL2.lib")
#pragma comment(lib, "glew32s.lib")
#pragma comment(lib, "opengl32.lib")
#else
#include <SDL2/SDL.h>
#endif

#include "defines.h"
#include "vertex_buffer.h"
#include "index_buffer.h"
#include "shader.h"
#include "camera.h"
#include "agent.h"
#include "rectangle.h"

int32 windowWidth = 1280;
int32 windowHeight = 720;

int colorUniformLocation;
int modelViewProjMatrixLocation;

std::list<Rectangle> rectangles;

// For windows add GLAPIENTRY behind void and window.h header
void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    std::cout << "[OpenGL Error] " << message << std::endl;
}

void render(float time, Camera &camera, Shader &shader)
{

    for (Rectangle &r : rectangles)
    {

        if (!colorUniformLocation != -1)
        {
            glUniform4f(colorUniformLocation, sinf(time) * sinf(time), 0.0f, 1.0f, 1.0f);
        }

        glm::mat4 modelViewProjMatrix = camera.getViewProj() * r.getModelMatrix();

        if (!modelViewProjMatrixLocation != -1)
        {
            glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);
        }

        if (!r.isSelected())
        {
            std::cout << "X: " << r.getPosX() << " Y: " << r.getPosY() << "not selected" << std::endl;
            r.render(shader);
        }
    }
}

void update(float ingameX, float ingameY)
{
    for (Rectangle &r : rectangles)
    {
        r.update(ingameX, ingameY);
    }
}

int main(int argc, char **argv)
{
    SDL_Window *window;
    SDL_Init(SDL_INIT_EVERYTHING);

    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE, 32);
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetSwapInterval(1);

#ifdef _DEBUG
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_FLAGS, SDL_GL_CONTEXT_DEBUG_FLAG);
#endif

    uint32 flags = SDL_WINDOW_OPENGL; // | SDL_WINDOW_BORDERLESS | SDL_WINDOW_FULLSCREEN_DESKTOP

    window = SDL_CreateWindow("MasterMap", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, windowWidth, windowHeight, flags);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    GLenum err = glewInit();
    if (err != GLEW_OK)
    {
        std::cout << "Error: " << glewGetErrorString(err) << std::endl;
        std::cin.get();
        return -1;
    }
    std::cout << "OpenGL version: " << glGetString(GL_VERSION) << std::endl;

#ifdef _DEBUG
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(openGLDebugCallback, 0);
#endif

    Shader shader("shaders/basic.vert", "shaders/basic.frag");
    shader.bind();

    uint64 perfCounterFrequency = SDL_GetPerformanceFrequency();
    uint64 lastCounter = SDL_GetPerformanceCounter();

    float32 delta = 0.0f;

    float32 camWidth = 1.0f * 16.0f * 2.0f;
    float32 camHeight = 1.0f * 9.0f * 2.0f;

    Camera camera(camWidth, camHeight);
    camera.translate(glm::vec3(0.0f, 0.0f, 5.0f));
    camera.update();

    colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
    modelViewProjMatrixLocation = glGetUniformLocation(shader.getShaderId(), "u_modelViewProj");

    // Wireframe that only displays outlines
    // glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    // glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    Vertex vertices[] = {
        Vertex{-0.5f, -0.5f, 0.0f},
        Vertex{-0.5f, 0.3f, 0.0f},
        Vertex{0.5f, 0.3f, 0.0f},
        Vertex{0.5f, -0.5f, 0.0f},
        Vertex{-0.3f, 0.3f, 0.0f},
        Vertex{-0.3f, 0.5f, 0.0f},
        Vertex{0.3f, 0.5f, 0.0f},
        Vertex{0.3f, 0.3f, 0.0f}};
    uint32 numVertices = 8;

    uint32 indices[] = {
        0, 1, 2,
        0, 2, 3,
        4, 5, 6,
        4, 6, 7};
    uint32 numIndices = 12;

    Rectangle rect(vertices, numVertices, indices, numIndices, 0, 0);
    Rectangle rect2(vertices, numVertices, indices, numIndices, 2, 0);
    Rectangle rect3(vertices, numVertices, indices, numIndices, -1, 0);
    Rectangle rect4(vertices, numVertices, indices, numIndices, 0, 2);
    Rectangle rect5(vertices, numVertices, indices, numIndices, 3, 0);
    Rectangle rect6(vertices, numVertices, indices, numIndices, -3, 0);

    rectangles.push_back(rect);
    rectangles.push_back(rect2);
    rectangles.push_back(rect3);
    rectangles.push_back(rect4);
    rectangles.push_back(rect5);
    rectangles.push_back(rect6);

    std::cout << "SIZE: " << sizeof(rectangles) << std::endl;

    bool buttonW = false;
    bool buttonS = false;
    bool buttonA = false;
    bool buttonD = false;

    bool isDragging = false;
    float currentMouseX = 0.0f;
    float currentMouseY = 0.0f;
    float lastMouseX = 0.0f;
    float lastMouseY = 0.0f;

    float screenX = 0.0f;
    float screenY = 0.0f;
    float ingameX = 0.0f;
    float ingameY = 0.0f;

    float time = 0.0f;
    bool close = false;
    
    while (!close)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                close = true;
            }
            if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    buttonW = true;
                    break;
                case SDLK_s:
                    buttonS = true;
                    break;
                case SDLK_a:
                    buttonA = true;
                    break;
                case SDLK_d:
                    buttonD = true;
                    break;
                }
            }
            if (event.type == SDL_KEYUP)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    buttonW = false;
                    break;
                case SDLK_s:
                    buttonS = false;
                    break;
                case SDLK_a:
                    buttonA = false;
                    break;
                case SDLK_d:
                    buttonD = false;
                    break;
                }
            }
            if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    isDragging = true;
                    lastMouseX = event.motion.x;
                    lastMouseY = event.motion.y;
                    currentMouseX = event.motion.x;
                    currentMouseY = event.motion.y;

                    // Calculate ingame coordinates

                    screenX = (event.motion.x * camWidth / windowWidth) - (camWidth / 2);
                    screenY = -1.0f * ((event.motion.y * camHeight / windowHeight) - (camHeight / 2));

                    ingameX = screenX - camera.getPosition()[0];
                    ingameY = screenY - camera.getPosition()[1];

                    std::cout << "ScreenX: " << screenX << " ScreenY: " << screenY << std::endl;
                    std::cout << "IngameX: " << ingameX << " IngameY: " << ingameY << std::endl;
                }
            }
            if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    isDragging = false;
                }
            }
            if (event.type == SDL_MOUSEMOTION)
            {
                if (isDragging)
                {
                    currentMouseX = event.motion.x;
                    currentMouseY = event.motion.y;

                    float offsetX = lastMouseX - currentMouseX;
                    float offsetY = currentMouseY - lastMouseY;

                    float sensitivity = 0.01f;

                    glm::vec3 movement = glm::vec3(offsetX * sensitivity, offsetY * sensitivity, 0.0f);
                    camera.translate(movement);

                    lastMouseX = currentMouseX;
                    lastMouseY = currentMouseY;
                }
            }
        }

        glClearColor(0.8f, 0.8f, 0.8f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        time += delta;

        if (buttonW)
        {
            camera.translate(glm::vec3(0.0f, 0.0f, -2.0f * delta));
        }
        if (buttonS)
        {
            camera.translate(glm::vec3(0.0f, 0.0f, 2.0f * delta));
        }
        if (buttonA)
        {
            camera.translate(glm::vec3(-2.0f * delta, 0.0f, 0.0f));
        }
        if (buttonD)
        {
            camera.translate(glm::vec3(2.0f * delta, 0.0f, 0.0f));
        }

        camera.update();

        for (Rectangle &r : rectangles)
        {
            r.rotate(360.0f * delta);
        }

        update(ingameX, ingameY);

        render(time, camera, shader);

        SDL_GL_SwapWindow(window);

        uint64 endCounter = SDL_GetPerformanceCounter();
        uint64 counterElapsed = endCounter - lastCounter;

        delta = ((float32)counterElapsed) / ((float32)perfCounterFrequency);
        uint32 FPS = (uint32)(((float32)perfCounterFrequency) / ((float32)counterElapsed));
        // std::cout << FPS << std::endl;
        lastCounter = endCounter;
    }

    return 0;
}
