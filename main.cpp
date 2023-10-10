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
#include "game_object.h"
#include "mesh.h"
#include "font.h"
#include "slab.h"
#include "button.h"
#include "converters.h"

int32 windowWidth = 1920;
int32 windowHeight = 1080;

// int32 windowWidth = 1280;
// int32 windowHeight = 720;

int colorUniformLocation;
int modelViewProjMatrixLocation;

std::list<std::unique_ptr<GameObject>> objects;
std::list<std::unique_ptr<Button>> buttons;

GameObject *selectedObjectPtr;

float ingameX = 0.0f;
float ingameY = 0.0f;

float screenX = 0.0f;
float screenY = 0.0f;

float32 camWidth = 1.0f * 16.0f * 2.0f;
float32 camHeight = 1.0f * 9.0f * 2.0f;

Camera camera(camWidth, camHeight);

Camera UICamera(camWidth, camHeight);

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

float32 delta = 0.0f;
float32 elapsUpdate = 0.0f;
float32 elapsCursorBlink = 0.0f;

// For windows add GLAPIENTRY behind void and window.h header
void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    std::cout << "[OpenGL Error] " << message << std::endl;
}

void update(float ingameX, float ingameY)
{
    elapsUpdate += delta;
    if (elapsUpdate >= 1.0f)
    {
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
        elapsUpdate = 0.0f;
    }

    for (auto &buttonPtr : buttons)
    {
        buttonPtr->update(screenX, screenY);
    }
}

void render(float time, Camera &camera, Shader &shader, Shader &fontShader)
{
    shader.bind();

    VertexBuffer highlightVB(standardVertices, standardNumVertices);
    IndexBuffer highlightIB(standardIndices, standardNumIndices, sizeof(uint32));

    glm::mat4 modelViewProjMatrix = camera.getViewProj() * glm::translate(glm::mat4(1.0f), glm::vec3((int)std::round(ingameX), (int)std::round(ingameY), 1.0f));

    glUniform4f(colorUniformLocation, 0.88f, 0.88f, 0.88f, 1.0f);
    glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);

    highlightVB.bind();
    highlightIB.bind();

    glDrawElements(GL_TRIANGLES, standardNumIndices, GL_UNSIGNED_INT, 0);

    highlightIB.unbind();
    highlightVB.unbind();

    for (auto &objectPtr : objects)
    {
        objectPtr->render(shader);
    }

    for (auto &buttonPtr : buttons)
    {
        buttonPtr->render(shader);
    }

    VertexBuffer editorVB(standardVertices, standardNumVertices);
    IndexBuffer editorIB(standardIndices, standardNumIndices, sizeof(uint32));

    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, camHeight / 6 - camHeight / 2.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(camWidth / 2.0f, camHeight / 3.0f, 1.0f));

    modelViewProjMatrix = UICamera.getViewProj() * modelMatrix;

    glUniform4f(colorUniformLocation, 0.1f, 0.1f, 0.1f, 1.0f);
    glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);

    editorVB.bind();
    editorIB.bind();

    glDrawElements(GL_TRIANGLES, standardNumIndices, GL_UNSIGNED_INT, 0);

    editorVB.unbind();
    editorIB.unbind();
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

    camera.translate(glm::vec3(0.0f, 0.0f, 5.0f));
    camera.update();

    UICamera.update();

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

    Mesh mesh(vertices, numVertices, indices, numIndices);
    std::unique_ptr<GameObject> agent = std::make_unique<Agent>(4, 0, 0, mesh, camera);

    objects.push_back(std::move(agent));

    Mesh buttonMesh(standardVertices, standardNumVertices, standardIndices, standardNumIndices);
    std::unique_ptr<Button> button = std::make_unique<Button>(0.0f, -7.0f, 1.0f, 1.0f, buttonMesh, UICamera);

    buttons.push_back(std::move(button));

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
                    for (auto &objectPtr : objects)
                    {
                        if (auto *agentPtr = dynamic_cast<Agent *>(objectPtr.get()))
                        {
                            // agentPtr->moveForward();
                        }
                    }
                    break;
                case SDLK_s:
                    break;
                case SDLK_a:

                    for (auto &objectPtr : objects)
                    {
                        if (auto *agentPtr = dynamic_cast<Agent *>(objectPtr.get()))
                        {
                            agentPtr->turnLeft();
                        }
                    }
                    break;
                case SDLK_d:

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
                    break;
                case SDLK_s:
                    break;
                case SDLK_a:
                    break;
                case SDLK_d:
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

                    // std::cout << "ScreenX: " << screenX << " ScreenY: " << screenY << std::endl;
                    // std::cout << "IngameX: " << ingameX << " IngameY: " << ingameY << std::endl;

                    // std::unique_ptr<GameObject> agent = std::make_unique<Agent>((int)std::round(ingameX), (int)std::round(ingameY), 0, mesh, camera);

                    // objects.push_back(std::move(agent));

                    for (auto &objectPtr : objects)
                    {
                        if (auto *agentPtr = dynamic_cast<Agent *>(objectPtr.get()))
                        {
                            if (agentPtr->getX() == (int)std::round(ingameX) && agentPtr->getY() == (int)std::round(ingameY))
                            {
                                if (selectedObjectPtr != nullptr)
                                {
                                    selectedObjectPtr->deselect();
                                }
                                agentPtr->select();
                                selectedObjectPtr = agentPtr;
                                break;
                            }
                            if (selectedObjectPtr != nullptr)
                            {
                                selectedObjectPtr->deselect();
                                selectedObjectPtr = nullptr;
                            }
                        }
                    }
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

                    glm::vec3 movement = glm::vec3(offsetX * camWidth / windowWidth * camera.getZoom(), offsetY * camHeight / windowHeight * camera.getZoom(), 0.0f);
                    camera.translate(movement);

                    // std::cout << camera.getZoom() << std::endl;

                    lastMouseX = currentMouseX;
                    lastMouseY = currentMouseY;
                }
            }
            else if (event.type == SDL_MOUSEWHEEL)
            {
                camera.changeZoom(-event.wheel.y);
            }
        }

        glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        time += delta;

        camera.update();

        update(ingameX, ingameY);

        shader.bind();
        render(time, camera, shader, fontShader);
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

        std::string scriptString = "HI";

        Agent *selectedAgentPtr = dynamic_cast<Agent *>(selectedObjectPtr);
        if (selectedAgentPtr != nullptr)
        {
            scriptString = selectedAgentPtr->getScript();

            int lineIndex = 0;
            std::string substr;
            glm::vec2 sumWidth(0.0f, 0.0f);

            while (scriptString.length() > 0)
            {
                int strPos = 0;

                while (scriptString[strPos] != '\n')
                {
                    strPos++;
                }

                substr = scriptString.substr(0, strPos);
                scriptString = scriptString.substr(strPos + 1);

                sumWidth = font.drawString(windowWidth / 4.0f + 20.0f, (2.0f * windowHeight / 3.0f) + (40.0f * lineIndex) + 40.0f, substr.c_str(), &fontShader);

                lineIndex++;
            }

            fontShader.unbind();

            shader.bind();

            VertexBuffer cursorVB(standardVertices, standardNumVertices);
            IndexBuffer cursorIB(standardIndices, standardNumIndices, sizeof(uint32));

            // std::cout << substr.length() << std::endl;

            glm::mat4 modelMatrix = glm::mat4(1.0f);
            // modelMatrix = glm::translate(modelMatrix, glm::vec3(- camWidth / 4.0f + (((40.0f + 22.0f + (44.0f * substr.length())) / windowWidth) * camWidth / 2.0f), -camHeight / 6.0f + (-((40.0f + (lineIndex * 60.0f)) / windowHeight) * camHeight / 2.0f), 0.0f));

            // float coord_x = (windowWidth / 4.0f) + (sumWidth) + 20.0f;
            float coord_x = sumWidth[0];
            float coord_y = (2.0f * windowHeight / 3.0f) + (40.0f * (lineIndex - 1)) + 40.0f;

            // float coord_x = windowWidth / ;
            // float coord_y = 100.0f;

            std::cout << coord_x << std::endl;
            std::cout << coord_y << std::endl;

            glm::vec2 coords = SDL_to_OPENGL_UI(glm::vec2(coord_x, coord_y), (float)windowWidth, (float)windowHeight, camWidth, camHeight);

            std::cout << coords[0] << " " << coords[1] << std::endl;

            coords[0] += 0.2f;  // 0.15f
            coords[1] += 0.22f; // should have calculated value

            std::cout << coords[0] << " " << coords[1] << std::endl;

            modelMatrix = glm::translate(modelMatrix, glm::vec3(coords, 0.0f));
            modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.5f, 1.0f));

            glm::mat4 modelViewProjMatrix = UICamera.getViewProj() * modelMatrix;

            glUniformMatrix4fv(glGetUniformLocation(shader.getShaderId(), "u_modelViewProj"), 1, GL_FALSE, &modelViewProjMatrix[0][0]);
            float val = (sinf(time * 4) * 0.5f * 0.9f) + 0.55f;
            glUniform4f(glGetUniformLocation(shader.getShaderId(), "u_color"), val, val, val, 1.0f);

            cursorVB.bind();
            cursorIB.bind();

            glDrawElements(GL_TRIANGLES, standardNumIndices, GL_UNSIGNED_INT, 0);

            cursorIB.unbind();
            cursorVB.unbind();

            shader.unbind();
        }

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