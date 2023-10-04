#define AGENT_FACING_UP 0
#define AGENT_FACING_LEFT 1
#define AGENT_FACING_DOWN 2
#define AGENT_FACING_RIGHT 3

#define AGENT_ROTATION_UP 0.0f
#define AGENT_ROTATION_LEFT 90.0f
#define AGENT_ROTATION_DOWN 180.0f
#define AGENT_ROTATION_RIGHT 270.0f

#include "libs/glm/glm.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"

#include "defines.h"
#include "vertex_buffer.h"
#include "index_buffer.h"

class Agent{
    private:
        int positionX;
        int positionY;
        int positionZ;
        int facing;
        float rotation;
        float width;
        float height;

        glm::mat4 modelMatrix;

        Vertex vertices[4];
        uint32 indices[6];
        VertexBuffer vertexBuffer;
        IndexBuffer indexBuffer;


    public:

        Agent(int x, int y, int z, float rotation = 0.0f, float width = 1.0f, float height = 1.0f)
        : positionX(x), positionY(y), positionZ(z), rotation(rotation), width(width), height(height)
        , vertices{{-0.5f, -0.5f, 0.0f},
                   {-0.5f, 0.5f, 0.0f},
                   {0.5f, 0.5f, 0.0f},
                   {0.5f, -0.5f, 0.0f}}, vertexBuffer(vertices, 4), indices{0,1,2,0,2,3}, indexBuffer(indices, 6, sizeof(uint32)){
            facing = AGENT_FACING_UP;

            updateModelMatrix();
        }

        void moveForward(){

        }

        void turnLeft(){

        }

        void turnRight(){

        }

        void updateModelMatrix() {
        // Update the model matrix based on object's position, rotation, and scale
        modelMatrix = glm::mat4(1.0f); // Start with an identity matrix
        modelMatrix = glm::translate(modelMatrix, glm::vec3(positionX, positionY, positionZ));
        modelMatrix = glm::rotate(modelMatrix, glm::radians(rotation), glm::vec3(0.0f, 0.0f, 1.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(width, height, 1.0f));
        }

        glm::mat4 getModelMatrix() const {
            return modelMatrix;
        }

        VertexBuffer getVertexBuffer(){
            return vertexBuffer;
        }

        IndexBuffer getIndexBuffer(){
            return indexBuffer;
        }
};