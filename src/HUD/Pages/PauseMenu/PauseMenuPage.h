#pragma once

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/Button.h"
#include "HUD/CoordsConverter.h"

class PauseMenuPage : public Page {

public:
    const char* _backgroundImagePath = "res/Images/pauseBackground.png";

    ImageRenderer _logo;

    std::shared_ptr<Button> _resumeButton;
    std::shared_ptr<Button> _settingsButton;
    std::shared_ptr<Button> _quitButton;
    std::shared_ptr<Button> _mainMenuButton;

    void Init() override;
    void SetupButtons();
    void Update() override;
};

