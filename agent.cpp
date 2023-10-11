#include "agent.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>

#include <SDL2/SDL.h>

Agent::Agent(int x, int y, int z, Mesh &mesh, Camera &camera, RGBAColor color, Script script) : GameObject(x, y, z, mesh, camera, color), script(script) 
{
    
}

void Agent::update()
{
    if (isSelected())
    {
        state = INACTIVE;

        
        
        
        // SHOULDNT HANDLE KEYINPUTS HERE CAUSE UPDATE IS CALLED ONCE A SECOND
        
        
        const uint8* state = SDL_GetKeyboardState(nullptr);

        if (state[SDL_SCANCODE_BACKSPACE])
        {
            // std::vector<std::string> lines = getScript().getLinesVector();
            std::cout << "BACKSPACE" << std::endl;
        }
    }
    else
    {
        state = AUTOMATIC;
    }

    if (state == AUTOMATIC)
    {
        std::vector<std::string> strings = script.getLinesVector();

        std::string str = strings.at(script.getPC());

        if (strings.at(script.getPC()).length() > 0)
        {

            if (str == "move")
            {
                moveForward();
            }
            if (str == "left")
            {
                turnLeft();
            }
            if (str == "right")
            {
                turnRight();
            }

            if (script.getPC() >= strings.size() - 1)
            {
                script.setPC(0);
                script.setCursor(0);
            }
            else
            {
                script.setPC(script.getPC() + 1);
                script.setCursor(script.getPC());
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

Script Agent::getScript()
{
    return script;
}