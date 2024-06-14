#pragma once

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/Button.h"
#include "Managers/GameManager.h"
#include <memory>

class MainMenuPage : public Page {
public:
    void Init() override;
    void Update() override;

    const char* _backgroundImagePath = "res/Images/MainMenu/mainMenu.png";

    std::shared_ptr<Button> _startButton;
    std::shared_ptr<Button> _settingsButton;
    std::shared_ptr<Button> _creditsButton;
    std::shared_ptr<Button> _quitButton; 

private:
    void SetupButtons();
    void SettingsTest();
    void CreditsButtonOnClick();
    void QuitButtonOnClick();
};
