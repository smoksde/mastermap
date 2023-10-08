#include "agent.h"

#include <iostream>
#include <cmath>

Agent::Agent(int x, int y, int z, Mesh &mesh) : GameObject(x, y, z, mesh)
{
}

void Agent::update()
{
    GameObject::update();
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