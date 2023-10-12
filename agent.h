#pragma once

#include "game_object.h"
#include "defines.h"
#include "string.h"
#include "script.h"

enum AgentState
{
    INACTIVE,
    AUTOMATIC,
    MANUAL
};

class Agent : public GameObject
{
private:
    Script script;
    AgentState state = INACTIVE;
public:
    Agent(int x, int y, int z, Mesh &mesh, Camera &camera, RGBAColor color, Script script);
    void update() override;
    void render(Shader &shader) override;
    void moveForward();
    void turnLeft();
    void turnRight();
    Script& getScript();
};