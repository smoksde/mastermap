#include "script.h"

#include <sstream>

Script::Script(std::string scriptString)
    : scriptString(scriptString)
{
    updateLinesVector();
}

std::string Script::getScriptString()
{
    return scriptString;
}

std::vector<std::string> Script::getLinesVector()
{
    return linesVector;
}

void Script::updateLinesVector()
{
    std::istringstream stringstream(scriptString);

    std::string line;

    std::vector<std::string> strings;

    // Split the string using '\n' as the delimiter
    while (std::getline(stringstream, line, '\n'))
    {
        strings.push_back(line);
    }

    linesVector = strings;
}

int Script::getPC()
{
    return pc;
}

int Script::getCursor()
{
    return cursor;
}

void Script::setPC(int newPC)
{
    pc = newPC;
}

void Script::setCursor(int newCursor)
{
    cursor = newCursor;
}