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

        int leftAmount = 0;
        int rightAmount = 0;

        if (leftPtr != nullptr)
        {
            leftAmount = leftPtr->getAmount();
        }

        if (rightPtr != nullptr)
        {
            rightAmount = rightPtr->getAmount();
        }

        int take = std::min(targetPtr->getCapacity() - targetPtr->getAmount(), leftAmount + rightAmount);
        targetPtr->setAmount(targetPtr->getAmount() + take);

        int leftTake = 0;
        int rightTake = 0;

        while(take > 0)
        {
            if (toggler)
            {
                if(leftAmount - leftTake > 0)
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
                if(rightAmount - rightTake > 0)
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
            leftPtr->setAmount(leftPtr->getAmount() - leftTake);
        }
        if (rightTake > 0)
        {
            rightPtr->setAmount(rightPtr->getAmount() - rightTake);
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

    int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
    int modelViewProjMatrixLocation = glGetUniformLocation(shader.getShaderId(), "u_modelViewProj");

    // specific model matrix for bar
    glm::mat4 innerModelMatrix = glm::mat4(1.0f);
    innerModelMatrix = glm::translate(innerModelMatrix, glm::vec3(0.0f, 0.2f, 0.0f));
    innerModelMatrix = glm::scale(innerModelMatrix, glm::vec3(0.2f, 0.2f, 1.0f));
    innerModelMatrix = glm::rotate(innerModelMatrix, float(-45.0f * (M_PI / 180.0f)), DEFAULT_ROTATION_VECTOR);


    glm::mat4 modelViewProjMatrix = camera.getViewProj() * getModelMatrix() * innerModelMatrix;
    
    glUniform4f(colorUniformLocation, 1.0f, 1.0f, 1.0f, 1.0f);
    glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);

    shader.bind();

    arrowMesh.bind();

    // Perform rendering using OpenGL (e.g., glDrawElements)
    glDrawElements(GL_TRIANGLES, arrowMesh.getNumIndices(), GL_UNSIGNED_INT, 0);

    // Unbind buffers when done
    arrowMesh.unbind();

    colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
    modelViewProjMatrixLocation = glGetUniformLocation(shader.getShaderId(), "u_modelViewProj");

    // specific model matrix for bar
    innerModelMatrix = glm::mat4(1.0f);
    innerModelMatrix = glm::translate(innerModelMatrix, glm::vec3(-0.3f, 0.0f, 0.0f));
    innerModelMatrix = glm::scale(innerModelMatrix, glm::vec3(0.2f, 0.2f, 1.0f));
    innerModelMatrix = glm::rotate(innerModelMatrix, float(-135.0f * (M_PI / 180.0f)), DEFAULT_ROTATION_VECTOR);


    modelViewProjMatrix = camera.getViewProj() * getModelMatrix() * innerModelMatrix;
    
    glUniform4f(colorUniformLocation, 1.0f, 1.0f, 1.0f, 1.0f);
    glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);

    shader.bind();

    arrowMesh.bind();

    // Perform rendering using OpenGL (e.g., glDrawElements)
    glDrawElements(GL_TRIANGLES, arrowMesh.getNumIndices(), GL_UNSIGNED_INT, 0);

    // Unbind buffers when done
    arrowMesh.unbind();

    colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
    modelViewProjMatrixLocation = glGetUniformLocation(shader.getShaderId(), "u_modelViewProj");

    // specific model matrix for bar
    innerModelMatrix = glm::mat4(1.0f);
    innerModelMatrix = glm::translate(innerModelMatrix, glm::vec3(0.3f, 0.0f, 0.0f));
    innerModelMatrix = glm::scale(innerModelMatrix, glm::vec3(0.2f, 0.2f, 1.0f));
    innerModelMatrix = glm::rotate(innerModelMatrix, float(45.0f * (M_PI / 180.0f)), DEFAULT_ROTATION_VECTOR);


    modelViewProjMatrix = camera.getViewProj() * getModelMatrix() * innerModelMatrix;
    
    glUniform4f(colorUniformLocation, 1.0f, 1.0f, 1.0f, 1.0f);
    glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);

    shader.bind();

    arrowMesh.bind();

    // Perform rendering using OpenGL (e.g., glDrawElements)
    glDrawElements(GL_TRIANGLES, arrowMesh.getNumIndices(), GL_UNSIGNED_INT, 0);

    // Unbind buffers when done
    arrowMesh.unbind();
}