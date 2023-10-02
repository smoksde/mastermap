#include <iostream>
#include <cmath>
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

// For windows add GLAPIENTRY behind void and window.h header
void openGLDebugCallback(GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam){
    std::cout << "[OpenGL Error] " << message << std::endl;
}

int main(int argc, char** argv) {
    SDL_Window* window;
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

    window = SDL_CreateWindow("MasterMap", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, flags);
    SDL_GLContext glContext = SDL_GL_CreateContext(window);

    GLenum err = glewInit();
    if(err != GLEW_OK){
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

    Vertex vertices[] = {
        Vertex{-0.5f, -0.5f, 0.0f,
        0.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 1.0f},
        Vertex{-0.5f, 0.5f, 0.0f,
        0.0f, 1.0f,
        0.0f, 1.0f, 0.0f, 1.0f},
        Vertex{0.5f, -0.5f, 0.0f,
        1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 1.0f},
        Vertex{0.5f, 0.5f, 0.0f,
        1.0f, 1.0f,
        1.0f, 0.0f, 0.0f, 1.0f}
    };
    uint32 numVertices = 4;

    uint32 indices[] = {
        0, 1, 2,
        1, 2, 3
    };
    uint32 numIndices = 6;

    IndexBuffer indexBuffer(indices, numIndices, sizeof(uint32));

    VertexBuffer vertexBuffer(vertices, numVertices);
    vertexBuffer.unbind(); // can be commented out?

    int32 textureWidth = 0;
    int32 textureHeight = 0;
    int32 bitsPerPixel = 0;
    stbi_set_flip_vertically_on_load(true);
    auto textureBuffer = stbi_load("graphics/logo.png", &textureWidth, &textureHeight, &bitsPerPixel, 4);

    GLuint textureId;
    glGenTextures(1, &textureId);
    glBindTexture(GL_TEXTURE_2D, textureId);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR); // if pixel look is wanted use GL_NEAREST
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, textureWidth, textureHeight, 0, GL_RGBA, GL_UNSIGNED_BYTE, textureBuffer);
    glBindTexture(GL_TEXTURE_2D, 0);

    if(textureBuffer){
        stbi_image_free(textureBuffer);
    }

    Shader shader("shaders/basic.vert", "shaders/basic.frag");
    shader.bind();

    uint64 perfCounterFrequency = SDL_GetPerformanceFrequency();
    uint64 lastCounter = SDL_GetPerformanceCounter();

    float32 delta = 0.0f;

    int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
    if(!colorUniformLocation != -1){
        glUniform4f(colorUniformLocation, 1.0f, 0.0f, 1.0f, 1.0f);
    }

    int textureUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_texture");
    if(!textureUniformLocation != -1){
        glUniform1i(textureUniformLocation, 0);
    }

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::scale(model, glm::vec3(2.0f));

    Camera camera(90.0f, 800.0f, 600.0f);
    camera.translate(glm::vec3(0.0f, 0.0f, 5.0f));
    camera.update();

    glm::mat4 modelViewProj = camera.getViewProj() * model;

    int modelViewProjMatrixLocation = glGetUniformLocation(shader.getShaderId(), "u_modelViewProj");

    // Wireframe that only displays outlines
    /*glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);*/

    bool buttonW = false;
    bool buttonS = false;
    bool buttonA = false;
    bool buttonD = false;

    bool isDragging = false;
    float lastMouseX = 0.0f;
    float lastMouseY = 0.0f;

    float time = 0.0f;
    bool close = false;
    while(!close){

        SDL_Event event;
        while(SDL_PollEvent(&event)){
            if(event.type == SDL_QUIT){
                close = true;
            }
            if(event.type == SDL_KEYDOWN){
                switch(event.key.keysym.sym){
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
            if(event.type == SDL_KEYUP){
                switch(event.key.keysym.sym){
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
            if(event.type == SDL_MOUSEBUTTONDOWN){
                std::cout << "MOUSE DOWN" << std::endl;
                if(event.button.button == SDL_BUTTON_LEFT){
                    std::cout << "LEFT MOUSE BUTTON DOWN" << std::endl;
                    isDragging = true;
                    lastMouseX = event.motion.x;
                    lastMouseY = event.motion.y;
                }
            }
            if(event.type == SDL_MOUSEBUTTONUP){
                if(event.button.button == SDL_BUTTON_LEFT){
                    isDragging = false;
                }
            }
            if(event.type == SDL_MOUSEMOTION){
                if(isDragging){
                    float xOffset = lastMouseX - event.motion.x;
                    float yOffset = event.motion.y - lastMouseY;

                    const float sensitivity = 0.1f;
                    xOffset *= sensitivity;
                    yOffset *= sensitivity;

                    std::cout << xOffset << yOffset << std::endl;
                    glm::vec3 movement = glm::vec3(xOffset, yOffset, 0.0f);
                    camera.translate(movement * delta);
                }
            }
        }

        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);
        time += delta;

        if(buttonW){
            camera.translate(glm::vec3(0.0f, 0.0f, -2.0f * delta));
        }
        if(buttonS){
            camera.translate(glm::vec3(0.0f, 0.0f, 2.0f * delta));
        }
        if(buttonA){
            camera.translate(glm::vec3(-2.0f * delta, 0.0f, 0.0f));
        }
        if(buttonD){
            camera.translate(glm::vec3(2.0f * delta, 0.0f, 0.0f));
        }

        camera.update();

        model = glm::rotate(model, 1.0f*delta, glm::vec3(0,0,1));
        modelViewProj = camera.getViewProj() * model;

        if(!colorUniformLocation != -1){
            glUniform4f(colorUniformLocation, sinf(time) * sinf(time), 0.0f, 1.0f, 1.0f);
        }

        vertexBuffer.bind();
        indexBuffer.bind();
        glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProj[0][0]);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textureId);
        glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLE_STRIP, 0, numVertices);
        indexBuffer.unbind();
        vertexBuffer.unbind();

        SDL_GL_SwapWindow(window);

        uint64 endCounter = SDL_GetPerformanceCounter();
        uint64 counterElapsed = endCounter - lastCounter;

        delta = ((float32)counterElapsed) / ((float32)perfCounterFrequency);
        uint32 FPS = (uint32)(((float32)perfCounterFrequency) /((float32)counterElapsed));
        // std::cout << FPS << std::endl;
        lastCounter = endCounter;
    }

    glDeleteTextures(1, &textureId);

    return 0;
}
