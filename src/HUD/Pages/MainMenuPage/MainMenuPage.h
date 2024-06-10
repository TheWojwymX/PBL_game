//
// Created by Jacek on 10.06.2024.
//

#ifndef SANDBOX_MAINMENUPAGE_H
#define SANDBOX_MAINMENUPAGE_H

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/MainMenu/MainMenuStartButton.h"
#include "HUD/Buttons/MainMenu/MainMenuCreditsButton.h"
#include "HUD/Buttons/MainMenu/MainMenuSettingsButton.h"
#include "HUD/Buttons/MainMenu/MainMenuQuitButton.h"

class MainMenuPage : public Page {

public:

    glm::vec3 leftDownCorner = glm::vec3(-1.0, -1.0, 0.0);
    glm::vec3 rightTopCorner = glm::vec3(1.0, 1.0, 0.0);
    std::array<float, 32> _backgroundVertices{
            // positions          // colors           // texture coords
            rightTopCorner.x,  leftDownCorner.y, rightTopCorner.z,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            rightTopCorner.x, rightTopCorner.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            leftDownCorner.x, rightTopCorner.y, leftDownCorner.z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            leftDownCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    void Init() override;

    void Update() override;

    void SetVertices(const array<float, 32> &vertices) override;

    const char* _backgroundImagePath = "res/Images/MainMenu/mainMenu.png";

    shared_ptr<MainMenuStartButton> _startButton = make_shared<MainMenuStartButton>();
    shared_ptr<MainMenuSettingsButton> _settingsButton = make_shared<MainMenuSettingsButton>();
    shared_ptr<MainMenuCreditsButton> _creditsButton = make_shared<MainMenuCreditsButton>();
    shared_ptr<MainMenuQuitButton> _quitButton = make_shared<MainMenuQuitButton>();
};


#endif //SANDBOX_MAINMENUPAGE_H
