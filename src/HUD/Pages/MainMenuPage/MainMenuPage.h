#pragma once

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/MainMenu/MainMenuStartButton.h"
#include "HUD/Buttons/MainMenu/MainMenuCreditsButton.h"
#include "HUD/Buttons/MainMenu/MainMenuSettingsButton.h"
#include "HUD/Buttons/MainMenu/MainMenuQuitButton.h"

class MainMenuPage : public Page {

public:
    void Init() override;

    void Update() override;

    const char* _backgroundImagePath = "res/Images/MainMenu/mainMenu.png";

    shared_ptr<MainMenuStartButton> _startButton = make_shared<MainMenuStartButton>();
    shared_ptr<MainMenuSettingsButton> _settingsButton = make_shared<MainMenuSettingsButton>();
    shared_ptr<MainMenuCreditsButton> _creditsButton = make_shared<MainMenuCreditsButton>();
    shared_ptr<MainMenuQuitButton> _quitButton = make_shared<MainMenuQuitButton>();
};
