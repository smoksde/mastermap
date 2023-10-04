#include "vertex_buffer.h"
#include "index_buffer.h"
#include "defines.h"

class Rectangle {
private:
    VertexBuffer vertexBuffer;
    IndexBuffer indexBuffer;

    glm::mat4 modelMatrix;

    int32 posX;
    int32 posY;

    glm::vec3 translationVector;
    glm::vec3 scalingVector;

    float32 rotation;
    glm::vec3 rotationVector;

public:
    Rectangle(Vertex* vertices, uint32 numVertices, uint32* indices, uint32 numIndices, int32 posX = 0, int32 posY = 0)
        : vertexBuffer(vertices, numVertices), indexBuffer(indices, numIndices, sizeof(uint32)) {
            translationVector = glm::vec3((float32)posX, (float32)posY, 0.0f);
            scalingVector = glm::vec3(1.0f, 1.0f, 1.0f);
            rotationVector = glm::vec3(0.0f, 1.0f, 0.0f);
            rotation = 0.0f;

            updateModelMatrix();
        }

    void updateModelMatrix(){
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::translate(modelMatrix, translationVector);
        // modelMatrix = glm::scale(modelMatrix, scalingVector);
        // modelMatrix = glm::rotate(modelMatrix, rotation, rotationVector);
    }

    glm::mat4 getModelMatrix(){
        return modelMatrix;
    }

    void render(Shader& shader) {

        shader.bind();

        // Bind vertex and index buffers
        vertexBuffer.bind();
        indexBuffer.bind();

        // Perform rendering using OpenGL (e.g., glDrawElements)
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

        // Unbind buffers when done
        indexBuffer.unbind();
        vertexBuffer.unbind();
    }
};