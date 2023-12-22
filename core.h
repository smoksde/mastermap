#pragma once

#include "game_object.h"

class Core : public GameObject
{
private:
    int amount = 0;
public:
    Core(int x, int y, int z, Mesh &mesh, Camera &camera, RGBAColor color, std::list<std::unique_ptr<GameObject>> &objects);
    void tick() override;
    void update(float elapseUpdate) override;
    void render(Shader &shader) override;
    int getAmount();
    void setAmount(int value);
};