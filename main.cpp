#include <iostream>
#include <memory>
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
#include "game_object.h"
#include "mesh.h"
#include "font.h"
#include "slab.h"
#include "button.h"

int32 windowWidth = 1920;
int32 windowHeight = 1080;

// int32 windowWidth = 1280;
// int32 windowHeight = 720;

int colorUniformLocation;
int modelViewProjMatrixLocation;

std::list<Rectangle> rectangles;
std::list<std::unique_ptr<GameObject>> objects;
std::list<std::unique_ptr<Button>> buttons;

float ingameX = 0.0f;
float ingameY = 0.0f;

float screenX = 0.0f;
float screenY = 0.0f;

float32 camWidth = 1.0f * 16.0f * 2.0f;
float32 camHeight = 1.0f * 9.0f * 2.0f;

glm::mat4 standardProj = glm::ortho(-camWidth / 2.0f, camWidth / 2.0f, -camHeight / 2.0f, camHeight / 2.0f, -1.0f, 1000.0f);

Vertex standardVertices[] = {
    Vertex{-0.5f, -0.5f, 0.0f},
    Vertex{-0.5f, 0.5f, 0.0f},
    Vertex{0.5f, 0.5f, 0.0f},
    Vertex{0.5f, -0.5f, 0.0f},
};
uint32 standardNumVertices = 4;

uint32 standardIndices[] = {
    0, 1, 2,
    0, 2, 3};
uint32 standardNumIndices = 6;

// For windows add GLAPIENTRY behind void and window.h header
void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    std::cout << "[OpenGL Error] " << message << std::endl;
}

void update(float ingameX, float ingameY)
{
    for (Rectangle &r : rectangles)
    {
        r.update(ingameX, ingameY);
    }

    for (auto &objectPtr : objects)
    {
        if (auto *agentPtr = dynamic_cast<Agent *>(objectPtr.get()))
        {
            agentPtr->update();
        }
        else
        {
            objectPtr->update();
        }
    }

    for (auto &buttonPtr : buttons)
    {
        buttonPtr->update(screenX, screenY);
    }
}

void render(float time, Camera &camera, Shader &shader)
{

    Vertex highlightVertices[] = {
        Vertex{-0.5f, -0.5f, 0.0f},
        Vertex{-0.5f, 0.5f, 0.0f},
        Vertex{0.5f, 0.5f, 0.0f},
        Vertex{0.5f, -0.5f, 0.0f},
    };
    uint32 highlightNumVertices = 4;

    uint32 highlightIndices[] = {
        0, 1, 2,
        0, 2, 3};
    uint32 highlightNumIndices = 6;

    VertexBuffer highlightVB(highlightVertices, highlightNumVertices);
    IndexBuffer highlightIB(highlightIndices, highlightNumIndices, sizeof(uint32));

    glm::mat4 modelViewProjMatrix = camera.getViewProj() * glm::translate(glm::mat4(1.0f), glm::vec3((int)std::round(ingameX), (int)std::round(ingameY), 1.0f));

    glUniform4f(colorUniformLocation, 0.88f, 0.88f, 0.88f, 1.0f);
    glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);

    highlightVB.bind();
    highlightIB.bind();

    glDrawElements(GL_TRIANGLES, highlightNumIndices, GL_UNSIGNED_INT, 0);

    highlightIB.unbind();
    highlightVB.unbind();

    for (Rectangle &r : rectangles)
    {

        if (!colorUniformLocation != -1)
        {
            glUniform4f(colorUniformLocation, 1.0f, (sinf(time) * sinf(time)) / 4 + 0.75f, 0.18f, 1.0f);
        }

        glm::mat4 modelViewProjMatrix = camera.getViewProj() * r.getModelMatrix();

        if (!modelViewProjMatrixLocation != -1)
        {
            glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);
        }

        if (!r.isSelected())
        {
            // std::cout << "X: " << r.getPosX() << " Y: " << r.getPosY() << "not selected" << std::endl;
            glUniform4f(colorUniformLocation, 1.0f, 0.78f, 0.18f, 1.0f);
        }
        r.render(shader);
    }

    for (auto &objectPtr : objects)
    {
        glUniform4f(colorUniformLocation, 0.1f, 0.1f, 0.1f, 1.0f);

        glm::mat4 modelViewProjMatrix = camera.getViewProj() * objectPtr->getModelMatrix();

        if (!modelViewProjMatrixLocation != -1)
        {
            glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);
        }

        objectPtr->render(shader);
    }

    for (auto &buttonPtr : buttons)
    {
        glUniform4f(colorUniformLocation, 0.1f, 0.1f, 0.1f, 1.0f);

        glm::mat4 modelViewProjMatrix = standardProj * buttonPtr->getModelMatrix();

        glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);

        buttonPtr->render(shader);
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

    // Activate blending for transparent objects
    // glEnable(GL_BLEND);
    // glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    Shader fontShader("shaders/font.vert", "shaders/font.frag");
    Shader shader("shaders/basic.vert", "shaders/basic.frag");
    shader.bind();

    Font font;
    font.initFont("fonts/roboto_mono/RobotoMono-SemiBold.ttf");

    uint64 perfCounterFrequency = SDL_GetPerformanceFrequency();
    uint64 lastCounter = SDL_GetPerformanceCounter();

    float32 delta = 0.0f;

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

    Mesh mesh(vertices, numVertices, indices, numIndices);
    std::unique_ptr<GameObject> agent = std::make_unique<Agent>(4, 0, 0, mesh);

    objects.push_back(std::move(agent));

    Mesh buttonMesh(standardVertices, standardNumVertices, standardIndices, standardNumIndices);
    std::unique_ptr<Button> button = std::make_unique<Button>(0.0f, -7.0f, 1.0f, 1.0f, buttonMesh);

    buttons.push_back(std::move(button));

    bool buttonW = false;
    bool buttonS = false;
    bool buttonA = false;
    bool buttonD = false;

    bool isDragging = false;
    float currentMouseX = 0.0f;
    float currentMouseY = 0.0f;
    float lastMouseX = 0.0f;
    float lastMouseY = 0.0f;

    float time = 0.0f;
    bool close = false;

    uint32 FPS = 0;
    std::string fpsString = "";
    float elaps = 1.0f;

    while (!close)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            if (event.type == SDL_QUIT)
            {
                close = true;
            }
            else if (event.type == SDL_KEYDOWN)
            {
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    buttonW = true;
                    for (auto &objectPtr : objects)
                    {
                        if (auto *agentPtr = dynamic_cast<Agent *>(objectPtr.get()))
                        {
                            agentPtr->moveForward();
                        }
                    }
                    break;
                case SDLK_s:
                    buttonS = true;
                    break;
                case SDLK_a:
                    buttonA = true;

                    for (auto &objectPtr : objects)
                    {
                        if (auto *agentPtr = dynamic_cast<Agent *>(objectPtr.get()))
                        {
                            agentPtr->turnLeft();
                        }
                    }
                    break;
                case SDLK_d:
                    buttonD = true;

                    for (auto &objectPtr : objects)
                    {
                        if (auto *agentPtr = dynamic_cast<Agent *>(objectPtr.get()))
                        {
                            agentPtr->turnRight();
                        }
                    }
                    break;
                }
            }
            else if (event.type == SDL_KEYUP)
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
            else if (event.type == SDL_MOUSEBUTTONDOWN)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    isDragging = true;
                    lastMouseX = event.motion.x;
                    lastMouseY = event.motion.y;
                    currentMouseX = event.motion.x;
                    currentMouseY = event.motion.y;

                    // Calculate ingame coordinates

                    screenX = (event.motion.x * camWidth * camera.getZoom() / windowWidth) - (camWidth * camera.getZoom() / 2);
                    screenY = -1.0f * ((event.motion.y * camHeight * camera.getZoom() / windowHeight) - (camHeight * camera.getZoom() / 2));

                    ingameX = screenX - camera.getPosition()[0];
                    ingameY = screenY - camera.getPosition()[1];

                    std::cout << "ScreenX: " << screenX << " ScreenY: " << screenY << std::endl;
                    std::cout << "IngameX: " << ingameX << " IngameY: " << ingameY << std::endl;

                    std::unique_ptr<GameObject> agent = std::make_unique<Agent>((int)std::round(ingameX), (int)std::round(ingameY), 0, mesh);

                    objects.push_back(std::move(agent));
                }
            }
            else if (event.type == SDL_MOUSEBUTTONUP)
            {
                if (event.button.button == SDL_BUTTON_LEFT)
                {
                    isDragging = false;
                }
            }
            else if (event.type == SDL_MOUSEMOTION)
            {
                currentMouseX = event.motion.x;
                currentMouseY = event.motion.y;

                screenX = (event.motion.x * camWidth * camera.getZoom() / windowWidth) - (camWidth * camera.getZoom() / 2);
                screenY = -1.0f * ((event.motion.y * camHeight * camera.getZoom() / windowHeight) - (camHeight * camera.getZoom() / 2));

                ingameX = screenX - camera.getPosition()[0];
                ingameY = screenY - camera.getPosition()[1];

                if (isDragging)
                {
                    float offsetX = lastMouseX - currentMouseX;
                    float offsetY = currentMouseY - lastMouseY;

                    float sensitivity = 0.01f;

                    glm::vec3 movement = glm::vec3(offsetX * camWidth / windowWidth * camera.getZoom(), offsetY * camHeight / windowHeight * camera.getZoom(), 0.0f);
                    camera.translate(movement);

                    std::cout << camera.getZoom() << std::endl;

                    lastMouseX = currentMouseX;
                    lastMouseY = currentMouseY;
                }
            }
            else if (event.type == SDL_MOUSEWHEEL)
            {
                camera.changeZoom(-event.wheel.y);
            }
        }

        glClearColor(0.9f, 0.9f, 0.9f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        time += delta;

        camera.update();

        update(ingameX, ingameY);

        shader.bind();
        render(time, camera, shader);
        shader.unbind();

        fontShader.bind();

        int w, h;
        SDL_GetWindowSize(window, &w, &h);
        glm::mat4 ortho = glm::ortho(0.0f, (float)w, (float)h, 0.0f);
        glUniformMatrix4fv(glGetUniformLocation(fontShader.getShaderId(), "u_modelViewProj"), 1, GL_FALSE, &ortho[0][0]);
        glDisable(GL_CULL_FACE);
        glEnable(GL_BLEND);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        glDisable(GL_DEPTH_TEST);

        // font.drawString(20.0f, 20.0f, "Ganymede", &fontShader);

        elaps += delta;
        if (elaps >= 1.0f)
        {
            fpsString = "FPS: ";
            fpsString.append(std::to_string(FPS));
            elaps = 0.0f;
        }
        font.drawString(12.0f, 40.0f, fpsString.c_str(), &fontShader);

        std::string xString = "X: ";
        xString.append(std::to_string((int)std::round(ingameX)));
        font.drawString(12.0f, 88.0f, xString.c_str(), &fontShader);

        std::string yString = "Y: ";
        yString.append(std::to_string((int)std::round(ingameY)));
        font.drawString(12.0f, 136.0f, yString.c_str(), &fontShader);

        fontShader.unbind();

        // glEnable(GL_CULL_FACE);
        // glEnable(GL_DEPTH_TEST);

        SDL_GL_SwapWindow(window);

        uint64 endCounter = SDL_GetPerformanceCounter();
        uint64 counterElapsed = endCounter - lastCounter;

        delta = ((float32)counterElapsed) / ((float32)perfCounterFrequency);
        FPS = (uint32)(((float32)perfCounterFrequency) / ((float32)counterElapsed));
        // std::cout << FPS << std::endl;
        lastCounter = endCounter;
    }

    return 0;
}