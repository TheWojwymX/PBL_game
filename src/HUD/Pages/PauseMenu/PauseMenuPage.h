#pragma once

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/PauseMenu/ResumeButton.h"
#include "HUD/Buttons/PauseMenu/QuitButton.h"
#include "HUD/Buttons/PauseMenu/SettingsButton.h"

class PauseMenuPage : public Page {

public:
    const char* _backgroundImagePath = "res/Images/pauseBackground.png";

    ResumeButton _resumeButton;
    QuitButton _quitButton;
    SettingsButton _settingsButton;

    ImageRenderer _logo;
    const char* _logoImagePath = "res/Images/logo.png";

    void Init() override;

    void Update() override;
};

