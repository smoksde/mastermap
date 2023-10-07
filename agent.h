#include "game_object.h"

class Agent : public GameObject
{
private:
public:
    Agent(int x, int y, int z, Mesh &mesh);
    void update() override;
    void moveForward();
    void turnLeft();
    void turnRight();
};