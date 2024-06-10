//
// Created by Jacek on 09.05.2024.
//

#ifndef SANDBOX_PAUSEMENUPAGE_H
#define SANDBOX_PAUSEMENUPAGE_H


#include "HUD/Pages/Page.h"
#include "HUD/Buttons/PauseMenu/ResumeButton.h"
#include "HUD/Buttons/PauseMenu/QuitButton.h"
#include "HUD/Buttons/PauseMenu/SettingsButton.h"

class PauseMenuPage : public Page {

public:

    const char* _backgroundImagePath = "res/Images/pauseBackground.png";

    glm::vec3 leftDownCorner = glm::vec3(-1.0, -1.0, 0.0);
    glm::vec3 rightTopCorner = glm::vec3(1.0, 1.0, 0.0);
    std::array<float, 32> _backgroundVertices{
            // positions          // colors           // texture coords
            rightTopCorner.x,  leftDownCorner.y, rightTopCorner.z,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            rightTopCorner.x, rightTopCorner.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            leftDownCorner.x, rightTopCorner.y, leftDownCorner.z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            leftDownCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    ResumeButton _resumeButton;
    QuitButton _quitButton;
    SettingsButton _settingsButton;

    ImageRenderer _logo;
    const char* _logoImagePath = "res/Images/logo.png";
    glm::vec3 logoDownCorner = glm::vec3(-0.2739583,0.587037, 0.0);
    glm::vec3 logoTopCorner = glm::vec3(0.2875,0.7814815, 0.0);
    std::array<float, 32> _logoVertices{
            // positions          // colors           // texture coords
            logoTopCorner.x,  logoDownCorner.y, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            logoTopCorner.x, logoTopCorner.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            logoDownCorner.x, logoTopCorner.y, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            logoDownCorner.x,  logoDownCorner.y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    void Init() override;

    void Update() override;

    void SetVertices(const array<float, 32> &vertices) override;

};


#endif //SANDBOX_PAUSEMENUPAGE_H
