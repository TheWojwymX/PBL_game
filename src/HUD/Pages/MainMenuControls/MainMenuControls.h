//
// Created by Jacek on 24.06.2024.
//

#ifndef SANDBOX_MAINMENUCONTROLS_H
#define SANDBOX_MAINMENUCONTROLS_H

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/Button.h"
#include "HUD/CoordsConverter.h"

class MainMenuControls : public Page {
public:
    void Init() override;
    void Update() override;

    const char* _backgroundImagePath = "res/Images/MainMenuSettings/MainMenuSettingsBackground.png";

    std::shared_ptr<Button> _backButton;

    ImageRenderer _buttonsBackground;
    ImageRenderer _logo;

private:
    void SetupButtons();
};


#endif //SANDBOX_MAINMENUCONTROLS_H
