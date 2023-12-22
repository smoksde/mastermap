#pragma once

#include "game_object.h"
#include "defines.h"
#include "string.h"
#include "script.h"

#include "source.h"
#include "core.h"
#include "storage.h"
#include "sink.h"

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

    int amount = 0;
    int capacity = 64;

    Mesh loadBarMesh;
public:
    Agent(int x, int y, int z, Mesh &mesh, Mesh &loadBarMesh, Camera &camera, RGBAColor color, Script script, std::list<std::unique_ptr<GameObject>> &objects);
    void tick() override;
    void update(float elapseUpdate) override;
    void render(Shader &shader) override;
    void moveForward();
    void turnLeft();
    void turnRight();
    void turn();
    void collect();
    void drop();
    int getAmount();
    Script& getScript();
};