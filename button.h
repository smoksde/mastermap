#include "game_object.h"

#include "libs/glm/glm.hpp"
#include "libs/glm/ext/matrix_transform.hpp"
#include "libs/glm/gtc/matrix_transform.hpp"

class Button
{
private:
    float x;
    float y;
    
    float width;
    float height;

    glm::vec3 translationVector;
    glm::vec3 scalingVector;
    glm::mat4 modelMatrix;

    Mesh mesh;
public:
    Button(float x, float y, float width, float height, Mesh &mesh);
    void update(float mouseX, float mouseY);
    void render(Shader &shader);
    void updateModelMatrix();
    glm::mat4 getModelMatrix();
    Mesh* getMesh();
};