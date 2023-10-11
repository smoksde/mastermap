#include "source.h"

Source::Source(int x, int y, int z, Mesh &mesh, Camera &camera, RGBAColor color) : GameObject(x, y, z, mesh, camera, color)
{

}

void Source::update()
{
    elaps++;
    if(elaps >= 4){
        amount++;
        elaps = 0;
    }
    GameObject::update();
}

void Source::render(Shader &shader)
{
    GameObject::render(shader);
}

int Source::getAmount()
{
    return amount;
}