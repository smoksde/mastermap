#pragma once

#include <list>
#include <memory>

#include "game_object.h"

class Sink : public GameObject
{
private:
    int amount = 0;
    int capacity = 120;

    int elaps = 0;
public:
    Sink(int x, int y, int z, Mesh &mesh, Camera &camera, RGBAColor color, std::list<std::unique_ptr<GameObject>> &objects);
    void tick() override;
    void update(float elapseUpdate) override;
    void render(Shader &shader) override;
    int getAmount();
    void setAmount(int value);
    int getCapacity();
    void setCapacity(int value);
};