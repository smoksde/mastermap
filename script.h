#pragma once

#include <string>
#include <vector>
#include <SDL2/SDL.h>

class Script
{
private:
    std::string scriptString;
    std::vector<std::string> linesVector;
    int pc = 0;
    int cursor = 0;
public:
    Script(std::string scriptString);
    std::string getScriptString();
    std::vector<std::string>& getLinesVector();
    void updateLinesVector();
    void updateScriptString();
    int getPC();
    int getCursor();
    void setPC(int newPC);
    void setCursor(int newCursor);

    void handleKeyInput(SDL_Keycode keyPressed);
};