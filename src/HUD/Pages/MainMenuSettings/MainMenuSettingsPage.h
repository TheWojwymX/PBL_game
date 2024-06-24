//
// Created by Jacek on 24.06.2024.
//

#ifndef SANDBOX_MAINMENUSETTINGSPAGE_H
#define SANDBOX_MAINMENUSETTINGSPAGE_H

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/Button.h"
#include "HUD/CoordsConverter.h"

class MainMenuSettingsPage : public Page {
public:
    void Init() override;
    void Update() override;

    const char* _backgroundImagePath = "res/Images/MainMenuSettings/MainMenuSettingsBackground.png";

    std::shared_ptr<Button> _musicMinusButton;
    std::shared_ptr<Button> _musicPlusButton;
    std::shared_ptr<Button> _sfxMinusButton;
    std::shared_ptr<Button> _sfxPlusButton;
    std::shared_ptr<Button> _discoButton;
    std::shared_ptr<Button> _controlsButton;
    std::shared_ptr<Button> _backButton;

    ImageRenderer _buttonsBackground;
    ImageRenderer _logo;

private:
    void SetupButtons();
};


#endif //SANDBOX_MAINMENUSETTINGSPAGE_H
