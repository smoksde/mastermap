#include "sink.h"

Sink::Sink(int x, int y, int z, Mesh &mesh, Camera &camera, RGBAColor color, std::list<std::unique_ptr<GameObject>> &objects) : GameObject(x, y, z, mesh, camera, color, objects)
{
    amount = 120;
}

void Sink::tick()
{
    elaps++;
    if(elaps >= 2){
        amount--;
        elaps = 0;
    }

    if (amount <= 0)
    {
        color = RGBAColor(1.0f, 0.0f, 1.0f, 1.0f);
        // Call game over function
    }

    setScalingVector(glm::vec3(amount/120.0f, amount/120.0f, 1.0f));

    GameObject::tick();
}

void Sink::update(float elapseUpdate)
{
    GameObject::update(elapseUpdate);
}

void Sink::render(Shader &shader)
{
    GameObject::render(shader);
}

int Sink::getAmount()
{
    return amount;
}

void Sink::setAmount(int value)
{
    amount = value;
}

int Sink::getCapacity()
{
    return capacity;
}

void Sink::setCapacity(int value)
{
    capacity = value;
}