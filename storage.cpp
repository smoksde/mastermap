#include "storage.h"

#include <list>
#include <memory>

Storage::Storage(int x, int y, int z, Mesh &mesh, Camera &camera, RGBAColor color, std::list<std::unique_ptr<GameObject>> &objects) : GameObject(x, y, z, mesh, camera, color, objects)
{

}

void Storage::tick()
{
    GameObject::tick();
}

void Storage::update(float elapseUpdate)
{
    GameObject::update(elapseUpdate);
}

void Storage::render(Shader &shader)
{
    GameObject::render(shader);
}

int Storage::getAmount()
{
    return amount;
}

void Storage::setAmount(int value)
{
    amount = value;
}

int Storage::getCapacity()
{
    return capacity;
}