#include "agent.h"

#include <iostream>
#include <sstream>
#include <string>
#include <vector>
#include <cmath>
#include <list>
#include <memory>

Agent::Agent(int x, int y, int z, Mesh &mesh, Mesh &loadBarMesh, Camera &camera, RGBAColor color, Script script, std::list<std::unique_ptr<GameObject>> &objects) : GameObject(x, y, z, mesh, camera, color, objects), script(script), loadBarMesh(loadBarMesh) 
{
    
}

void Agent::tick()
{
    GameObject::tick();

    if (isSelected())
    {
        state = INACTIVE;
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
            else if (str == "left")
            {
                turnLeft();
            }
            else if (str == "right")
            {
                turnRight();
            }
            else if (str == "turn")
            {
                turn();
            }
            else if (str == "collect")
            {
                collect();
            }
            else if (str == "drop")
            {
                drop();
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
}

void Agent::update(float elapseUpdate)
{
    GameObject::update(elapseUpdate);
}



void Agent::render(Shader &shader)
{
    GameObject::render(shader);

    int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
    int modelViewProjMatrixLocation = glGetUniformLocation(shader.getShaderId(), "u_modelViewProj");

    // specific model matrix for bar
    glm::mat4 innerModelMatrix = glm::mat4(1.0f);
    innerModelMatrix = glm::translate(innerModelMatrix, glm::vec3(0.0f, -0.2f, 0.0f));
    innerModelMatrix = glm::scale(innerModelMatrix, glm::vec3(std::min(std::max(0.1f, float(amount) / capacity * 0.8f), float(amount)), 0.1f, 1.0f));

    glm::mat4 modelViewProjMatrix = camera.getViewProj() * getModelMatrix() * innerModelMatrix;
    
    glUniform4f(colorUniformLocation, 1.0f, 0.525f, 0.525f, 1.0f);
    glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);

    shader.bind();

    loadBarMesh.bind();

    // Perform rendering using OpenGL (e.g., glDrawElements)
    glDrawElements(GL_TRIANGLES, loadBarMesh.getNumIndices(), GL_UNSIGNED_INT, 0);

    // Unbind buffers when done
    loadBarMesh.unbind();
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

void Agent::turn()
{
    int fac = getFacing();

    switch (fac)
    {
    case FACING_UP:
        setFacing(FACING_DOWN);
        break;
    case FACING_LEFT:
        setFacing(FACING_RIGHT);
        break;
    case FACING_DOWN:
        setFacing(FACING_UP);
        break;
    case FACING_RIGHT:
        setFacing(FACING_LEFT);
        break;
    default:
        break;
    }
}

void Agent::collect()
{
    int targetX = getX();
    int targetY = getY();

    switch(getFacing())
    {
        case FACING_UP:
            targetY++;
            break;
        case FACING_LEFT:
            targetX--;
            break;
        case FACING_DOWN:
            targetY--;
            break;
        case FACING_RIGHT:
            targetX++;
            break;
        default:
            break;
    }

    for (auto &object : objects)
    {
        if(object -> getX() == targetX && object -> getY() == targetY)
        {
            if (Source *sourcePtr = dynamic_cast<Source*>(object.get()))
            {
                int storage = capacity - amount;
                int take = std::min(storage, sourcePtr->getAmount());

                sourcePtr->setAmount(sourcePtr->getAmount() - take);
                amount += take;
            }

            if (Storage *storagePtr = dynamic_cast<Storage*>(object.get()))
            {
                int storage = capacity - amount;
                int take = std::min(storage, storagePtr->getAmount());

                storagePtr->setAmount(storagePtr->getAmount() - take);
                amount += take;
            }
        }
    }
}

void Agent::drop()
{
    int targetX = getX();
    int targetY = getY();

    switch(getFacing())
    {
        case FACING_UP:
            targetY++;
            break;
        case FACING_LEFT:
            targetX--;
            break;
        case FACING_DOWN:
            targetY--;
            break;
        case FACING_RIGHT:
            targetX++;
            break;
        default:
            break;
    }

    for (auto &object : objects)
    {
        if(object -> getX() == targetX && object -> getY() == targetY)
        {
            if (Core *corePtr = dynamic_cast<Core*>(object.get()))
            {
                corePtr->setAmount(corePtr->getAmount() + amount);
                amount = 0;
            }
            else if (Storage *storagePtr = dynamic_cast<Storage*>(object.get()))
            {
                int storage = storagePtr->getCapacity() - storagePtr->getAmount();
                int give = std::min(storage, amount);

                storagePtr->setAmount(storagePtr->getAmount() + give);
                amount -= give;
            }
            else if (Sink *sinkPtr = dynamic_cast<Sink*>(object.get()))
            {
                int storage = sinkPtr->getCapacity() - sinkPtr->getAmount();
                int give = std::min(storage, amount);

                sinkPtr->setAmount(sinkPtr->getAmount() + give);
                amount -= give;
            }
        }
    }
}

int Agent::getAmount()
{
    return amount;
}

Script& Agent::getScript()
{
    return script;
}