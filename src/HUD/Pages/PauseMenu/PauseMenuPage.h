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

    std::array<float, 32> _backgroundVertices{
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    ResumeButton _resumeButton;
    QuitButton _quitButton;
    SettingsButton _settingsButton;

    void Init() override;

    void Update() override;

    void SetVertices(const array<float, 32> &vertices) override;

};


#endif //SANDBOX_PAUSEMENUPAGE_H
