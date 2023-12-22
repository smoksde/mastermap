#include "core.h"

#include <list>
#include <memory>

Core::Core(int x, int y, int z, Mesh &mesh, Camera &camera, RGBAColor color, std::list<std::unique_ptr<GameObject>> &objects) : GameObject(x, y, z, mesh, camera, color, objects)
{

}

void Core::tick()
{
    GameObject::tick();
}

void Core::update(float elapseUpdate)
{
    GameObject::update(elapseUpdate);
}

void Core::render(Shader &shader)
{
    GameObject::render(shader);
}

int Core::getAmount()
{
    return amount;
}

void Core::setAmount(int value)
{
    amount = value;
}