#pragma once

#include <list>
#include <memory>

#include "game_object.h"
#include "storage.h"

class Splitter : public GameObject
{
private:
    bool toggler = true;

    Mesh arrowMesh;
protected:
public:
    Splitter(int x, int y, int z, Mesh &mesh, Mesh &arrowMesh, Camera &camera, RGBAColor color, std::list<std::unique_ptr<GameObject>> &objects);
    void tick() override;
    void update(float elapseUpdate) override;
    void render(Shader &shader) override;
};