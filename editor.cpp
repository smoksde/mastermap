#include "editor.h"

Editor::Editor(Mesh &mesh, Camera &camera)
    : mesh(mesh), camera(camera)
{
}

void Editor::update()
{
}

void Editor::render(Shader &shader, Shader &fontShader, GameObject *selectedObjectPtr)
{
    shader.bind();
    
    glm::mat4 modelMatrix = glm::mat4(1.0f);
    modelMatrix = glm::translate(modelMatrix, glm::vec3(0.0f, camera.getHeight() / 6 - camera.getHeight() / 2.0f, 0.0f));
    modelMatrix = glm::scale(modelMatrix, glm::vec3(camera.getWidth() / 2.0f, camera.getHeight() / 3.0f, 1.0f));

    glm::mat4 modelViewProjMatrix = camera.getViewProj() * modelMatrix;

    int colorUniformLocation = glGetUniformLocation(shader.getShaderId(), "u_color");
    int modelViewProjMatrixLocation = glGetUniformLocation(shader.getShaderId(), "u_modelViewProj");

    glUniform4f(colorUniformLocation, 48.0f / 255.0f, 10.0f / 255.0f, 36.0f / 255.0f, 1.0f);
    glUniformMatrix4fv(modelViewProjMatrixLocation, 1, GL_FALSE, &modelViewProjMatrix[0][0]);

    getMesh()->bind();

    glDrawElements(GL_TRIANGLES, getMesh()->getNumIndices(), GL_UNSIGNED_INT, 0);

    getMesh()->unbind();

    shader.unbind();

    /*Font font;
    font.initFont("fonts/roboto_mono/RobotoMono-SemiBold.ttf");

    std::string scriptString = "HI";

    int windowWidth = 1920;
    int windowHeight = 1080;

    Agent *selectedAgentPtr = dynamic_cast<Agent *>(selectedObjectPtr);
    if (selectedAgentPtr != nullptr)
    {
        scriptString = selectedAgentPtr->getScript();

        int lineIndex = 0;
        std::string substr;
        glm::vec2 sumWidth(0.0f, 0.0f);

        fontShader.bind();

        while (scriptString.length() > 0)
        {

            

            int strPos = 0;

            while (scriptString[strPos] != '\n')
            {
                strPos++;
            }

            substr = scriptString.substr(0, strPos);
            scriptString = scriptString.substr(strPos + 1);

            sumWidth = font.drawString(windowWidth / 4.0f + 20.0f, (2.0f * windowHeight / 3.0f) + (40.0f * lineIndex) + 40.0f, substr.c_str(), &fontShader);

            lineIndex++;

            
        }

        fontShader.unbind();

        shader.bind();

        // std::cout << substr.length() << std::endl;

        glm::mat4 modelMatrix = glm::mat4(1.0f);
        // modelMatrix = glm::translate(modelMatrix, glm::vec3(- camWidth / 4.0f + (((40.0f + 22.0f + (44.0f * substr.length())) / windowWidth) * camWidth / 2.0f), -camHeight / 6.0f + (-((40.0f + (lineIndex * 60.0f)) / windowHeight) * camHeight / 2.0f), 0.0f));

        // float coord_x = (windowWidth / 4.0f) + (sumWidth) + 20.0f;
        float coord_x = sumWidth[0];
        float coord_y = (2.0f * windowHeight / 3.0f) + (40.0f * (lineIndex - 1)) + 40.0f;

        // float coord_x = windowWidth / ;
        // float coord_y = 100.0f;

        // std::cout << coord_x << std::endl;
        // std::cout << coord_y << std::endl;

        glm::vec2 coords = SDL_to_OPENGL_UI(glm::vec2(coord_x, coord_y), (float)windowWidth, (float)windowHeight, camera.getWidth(), camera.getHeight());

        // std::cout << coords[0] << " " << coords[1] << std::endl;

        coords[0] += 0.2f;  // 0.15f
        coords[1] += 0.22f; // should have calculated value

        // std::cout << coords[0] << " " << coords[1] << std::endl;

        modelMatrix = glm::translate(modelMatrix, glm::vec3(coords, 0.0f));
        modelMatrix = glm::scale(modelMatrix, glm::vec3(0.3f, 0.5f, 1.0f));

        glm::mat4 modelViewProjMatrix = camera.getViewProj() * modelMatrix;

        glUniformMatrix4fv(glGetUniformLocation(shader.getShaderId(), "u_modelViewProj"), 1, GL_FALSE, &modelViewProjMatrix[0][0]);
        float val = 1.0f;
        glUniform4f(glGetUniformLocation(shader.getShaderId(), "u_color"), val, val, val, 1.0f);

        getMesh()->bind();

        glDrawElements(GL_TRIANGLES, getMesh()->getNumIndices(), GL_UNSIGNED_INT, 0);

        getMesh()->unbind();

        shader.unbind();
    }*/
}

Mesh *Editor::getMesh()
{
    return &mesh;
}