#include "script.h"

#include <sstream>
#include <iostream>
#include <string>

Script::Script(std::string scriptString)
    : scriptString(scriptString)
{
    updateLinesVector();
}

std::string Script::getScriptString()
{
    return scriptString;
}

std::vector<std::string> &Script::getLinesVector()
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

void Script::updateScriptString()
{
    // build the scriptstring according to whats in the lines vector
}

void Script::handleKeyInput(SDL_Keycode keyPressed)
{
    std::string line;
    int positionToInsert = cursor + 1; // Change this to the desired position
    std::string emptyLine = "";        // The empty line to insert
    switch (keyPressed)
    {
    case SDLK_RETURN:

        if (positionToInsert >= 0 && positionToInsert <= linesVector.size())
        {
            linesVector.insert(linesVector.begin() + positionToInsert, emptyLine);
            cursor++;
        }
        break;
    case SDLK_BACKSPACE:
        line = linesVector.at(cursor);
        std::cout << linesVector.at(cursor);
        if (line.length() > 0)
        {
            line.pop_back();
        }
        else
        {
            if (cursor > 0 && cursor < linesVector.size())
            {
                auto it = linesVector.begin() + cursor;
                linesVector.erase(it);
                cursor--;
            }
            else if (cursor == 0 && !linesVector.empty())
            {
                // linesVector.erase(linesVector.begin());
            }
            break;
            /*if (cursor > 0 && cursor < linesVector.size())
            {
                auto it = linesVector.begin() + cursor;
                linesVector.erase(it);
                cursor--;
            }*/
            // Go to upper line and remove the empty line
            /*if (cursor > 0)
            {
                auto it = linesVector.begin();
                std::advance(it, cursor + 1);
                linesVector.erase(it);
                cursor--;
            }*/
        }
        linesVector.at(cursor) = line;
        std::cout << linesVector.at(cursor);
        break;
    case SDLK_UP:
        if (cursor > 0)
            cursor--;
        break;
    case SDLK_DOWN:
        if (cursor < linesVector.size() - 1)
            cursor++;
        break;
    default:
        if (keyPressed >= SDLK_SPACE && keyPressed <= SDLK_DELETE)
        {
            char printableChar = static_cast<char>(keyPressed);
            linesVector.at(cursor).push_back(printableChar);
        }
        break;
    }

    pc = cursor;
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