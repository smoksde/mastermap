#pragma once

#include <string>
#include <vector>

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
    std::vector<std::string> getLinesVector();
    void updateLinesVector();
    int getPC();
    int getCursor();
    void setPC(int newPC);
    void setCursor(int newCursor);
};