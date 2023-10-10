#include "game_object.h"
#include "defines.h"
#include "string.h"

enum AgentState
{
    INACTIVE,
    AUTOMATIC,
    MANUAL
};

class Agent : public GameObject
{
private:
    std::string script = "move\nmove\nmmmmmmmmmmmmmmmmmmmmmmmmmmml\n";
    int pc = 0;
    AgentState state = INACTIVE;
public:
    Agent(int x, int y, int z, Mesh &mesh, Camera &camera);
    void update() override;
    void render(Shader &shader) override;
    void moveForward();
    void turnLeft();
    void turnRight();
    std::string getScript();
    void setScript(std::string str);
};