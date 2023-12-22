#include "sink.h"

Sink::Sink(int x, int y, int z, Mesh &mesh, Mesh &barMesh, Camera &camera, RGBAColor color, std::list<std::unique_ptr<GameObject>> &objects) : GameObject(x, y, z, mesh, camera, color, objects), barMesh(barMesh)
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

    GameObject::tick();
}

void Sink::update(float elapseUpdate)
{
    GameObject::update(elapseUpdate);
}

void Sink::render(Shader &shader)
{
    GameObject::render(shader);

    int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
    int modelViewProjMatrixLocation = glGetUniformLocation(shader.getShaderId(), "u_modelViewProj");

    // specific model matrix for bar
    glm::mat4 innerModelMatrix = glm::mat4(1.0f);
    innerModelMatrix = glm::translate(innerModelMatrix, glm::vec3(-(1.0f - float(amount) / capacity) / 2.0f, -0.4f, 0.0f));
    innerModelMatrix = glm::scale(innerModelMatrix, glm::vec3(float(amount) / capacity, 0.2f, 1.0f));

    glm::mat4 modelViewProjMatrix = camera.getViewProj() * getModelMatrix() * innerModelMatrix;
    
    glUniform4f(colorUniformLocation, 1.0f, 0.525f, 0.525f, 1.0f);
    glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);

    shader.bind();

    barMesh.bind();

    // Perform rendering using OpenGL (e.g., glDrawElements)
    glDrawElements(GL_TRIANGLES, barMesh.getNumIndices(), GL_UNSIGNED_INT, 0);

    // Unbind buffers when done
    barMesh.unbind();
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