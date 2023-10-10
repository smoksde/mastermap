#include "agent.h"

#include <iostream>
#include <cmath>

Agent::Agent(int x, int y, int z, Mesh &mesh, Camera &camera) : GameObject(x, y, z, mesh, camera)
{
}

void Agent::update()
{
    if (isSelected())
    {
        std::string str = getScript();

        if (str.length() > 0)
        {
            char character = str[pc];

            if (character == 'm')
            {
                moveForward();
            }
            if (character == 'l')
            {
                turnLeft();
            }
            if (character == 'r')
            {
                turnRight();
            }

            if (pc >= str.length() - 1)
            {
                pc = 0;
            }
            else
            {
                pc++;
            }
        }
    }
    GameObject::update();
}

void Agent::render(Shader &shader)
{
    GameObject::render(shader);
}

void Agent::moveForward()
{
    int fac = getFacing();

    switch (fac)
    {
    case FACING_UP:
        setY(getY() + 1);
        break;
    case FACING_LEFT:
        setX(getX() - 1);
        break;
    case FACING_DOWN:
        setY(getY() - 1);
        break;
    case FACING_RIGHT:
        setX(getX() + 1);
        break;
    default:
        break;
    }
}

void Agent::turnLeft()
{
    int fac = getFacing();

    switch (fac)
    {
    case FACING_UP:
        setFacing(FACING_LEFT);
        break;
    case FACING_LEFT:
        setFacing(FACING_DOWN);
        break;
    case FACING_DOWN:
        setFacing(FACING_RIGHT);
        break;
    case FACING_RIGHT:
        setFacing(FACING_UP);
        break;
    default:
        break;
    }
}

void Agent::turnRight()
{
    int fac = getFacing();

    switch (fac)
    {
    case FACING_UP:
        setFacing(FACING_RIGHT);
        break;
    case FACING_LEFT:
        setFacing(FACING_UP);
        break;
    case FACING_DOWN:
        setFacing(FACING_LEFT);
        break;
    case FACING_RIGHT:
        setFacing(FACING_DOWN);
        break;
    default:
        break;
    }
}

void Agent::setScript(std::string str)
{
    script = str;
}

std::string Agent::getScript()
{
    return script;
}