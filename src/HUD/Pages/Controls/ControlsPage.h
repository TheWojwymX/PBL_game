//
// Created by Jacek on 24.06.2024.
//

#ifndef SANDBOX_CONTROLSPAGE_H
#define SANDBOX_CONTROLSPAGE_H

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/Button.h"
#include "HUD/CoordsConverter.h"

class ControlsPage : public Page {
public:
    void Init() override;
    void Update() override;

    std::shared_ptr<Button> _backButton;

    ImageRenderer _buttonsBackground;
    ImageRenderer _logo;

    const char* _backgroundMainMenuImagePath = "res/Images/MainMenuSettings/MainMenuSettingsBackground.png";
    ImageRenderer _backgroundMainMenu;

    int _mode = 0; //0 - settings from main menu, 1 - settings from pause menu

private:
    void SetupButtons();
};


#endif //SANDBOX_CONTROLSPAGE_H
