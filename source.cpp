#include "source.h"

Source::Source(int x, int y, int z, Mesh &mesh, Mesh &innerMesh, Camera &camera, RGBAColor color, std::list<std::unique_ptr<GameObject>> &objects) : GameObject(x, y, z, mesh, camera, color, objects), innerMesh(innerMesh)
{
    
}

void Source::tick()
{
    elaps++;
    if(elaps >= 4){
        if(amount < capacity)
        {
            amount++;
        }
        elaps = 0;
    }
    innerScale = std::min(float(amount) / capacity, 1.0f);
    innerScale = std::max(innerScale, 0.0f);
    innerScale *= 0.8;
}

void Source::update(float elapseUpdate)
{
    
}

void Source::render(Shader &shader)
{
    GameObject::render(shader);

    int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
    int modelViewProjMatrixLocation = glGetUniformLocation(shader.getShaderId(), "u_modelViewProj");

    // specific model matrix for inner
    glm::mat4 innerModelMatrix = glm::mat4(1.0f);
    innerModelMatrix = glm::translate(innerModelMatrix, getTranslationVector());
    innerModelMatrix = glm::scale(innerModelMatrix, glm::vec3(innerScale, innerScale, 1.0f));

    glm::mat4 modelViewProjMatrix = camera.getViewProj() * innerModelMatrix;

    glUniform4f(colorUniformLocation, 1.0f, 0.525f, 0.525f, 1.0f);
    glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);

    shader.bind();

    innerMesh.bind();

    // Perform rendering using OpenGL (e.g., glDrawElements)
    glDrawElements(GL_TRIANGLES, innerMesh.getNumIndices(), GL_UNSIGNED_INT, 0);

    // Unbind buffers when done
    innerMesh.unbind();
}

int Source::getAmount()
{
    return amount;
}

void Source::setAmount(int value)
{
    amount = value;
}