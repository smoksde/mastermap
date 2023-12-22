#pragma once

#include "game_object.h"

class Source : public GameObject
{
private:
    int amount = 0;
    int capacity = 8;
    int elaps = 0;
    float innerScale = 0.0f;
    Mesh innerMesh;
public:
    Source(int x, int y, int z, Mesh &mesh, Mesh &innerMesh, Camera &camera, RGBAColor color, std::list<std::unique_ptr<GameObject>> &objects);
    void tick() override;
    void update(float elapseUpdate) override;
    void render(Shader &shader) override;
    int getAmount();
    void setAmount(int value);
};