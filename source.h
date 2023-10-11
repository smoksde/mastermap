#pragma once

#include "game_object.h"

class Source : public GameObject
{
private:
    int amount = 0;
    int elaps = 0;
public:
    Source(int x, int y, int z, Mesh &mesh, Camera &camera, RGBAColor color);
    void update() override;
    void render(Shader &shader) override;
    int getAmount();
};