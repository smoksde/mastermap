#include "merger.h"

Merger::Merger(int x, int y, int z, Mesh &mesh, Mesh &arrowMesh, Camera &camera, RGBAColor color, std::list<std::unique_ptr<GameObject>> &objects) : GameObject(x, y, z, mesh, camera, color, objects), arrowMesh(arrowMesh)
{

}

void Merger::tick()
{

    int targetX = getX();
    int targetY = getY();
    int leftX = getX();
    int leftY = getY();
    int rightX = getX();
    int rightY = getY();

    switch(getFacing())
    {
        case FACING_UP:
            targetY++;
            leftX--;
            rightX++;
            break;
        case FACING_LEFT:
            targetX--;
            leftY--;
            rightY++;
            break;
        case FACING_DOWN:
            targetY--;
            leftX++;
            rightX--;
            break;
        case FACING_RIGHT:
            targetX++;
            leftY++;
            rightY--;
            break;
        default:
            break;
    }

    Storage *targetPtr = nullptr;
    Storage *leftPtr = nullptr;
    Storage *rightPtr = nullptr;

    for (auto &object : objects)
    {
        if (object->getX() == targetX && object->getY() == targetY)
        {
            if (targetPtr = dynamic_cast<Storage*>(object.get()))
            {

            }
        }
        else if (object->getX() == leftX && object->getY() == leftY)
        {
            if (leftPtr = dynamic_cast<Storage*>(object.get()))
            {

            }
        }
        else if (object->getX() == rightX && object->getY() == rightY)
        {
            if (rightPtr = dynamic_cast<Storage*>(object.get()))
            {

            }
        }
    }

    // TODO: Below here the code has to be adapted to merger instead splitter functionality

    if (targetPtr != nullptr)
    {

        int leftSpace = 0;
        int rightSpace = 0;

        if (leftPtr != nullptr)
        {
            leftSpace = leftPtr->getCapacity() - leftPtr->getAmount();
        }

        if (rightPtr != nullptr)
        {
            rightSpace = rightPtr->getCapacity() - rightPtr->getAmount();
        }

        int take = std::min(targetPtr->getAmount(), leftSpace + rightSpace);
        targetPtr->setAmount(targetPtr->getAmount() - take);

        int leftTake = 0;
        int rightTake = 0;

        while(take > 0)
        {
            if (toggler)
            {
                if(leftSpace - leftTake > 0)
                {
                    take--;
                    leftTake++;
                    toggler = !toggler;
                }
                else
                {
                    rightTake += take;
                    take = 0;
                }
            }
            else
            {
                if(rightSpace - rightTake > 0)
                {
                    take--;
                    rightTake++;
                    toggler = !toggler;
                }
                else
                {
                    leftTake += take;
                    take = 0;
                }
            }
        }

        if (leftTake > 0)
        {
            leftPtr->setAmount(leftPtr->getAmount() + leftTake);
        }
        if (rightTake > 0)
        {
            rightPtr->setAmount(rightPtr->getAmount() + rightTake);
        }
    }

    GameObject::tick();
}

void Merger::update(float elapseUpdate)
{
    GameObject::update(elapseUpdate);
}

void Merger::render(Shader &shader)
{
    GameObject::render(shader);
}