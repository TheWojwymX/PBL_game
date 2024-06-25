//
// Created by Jacek on 24.06.2024.
//

#ifndef SANDBOX_SETTINGSPAGE_H
#define SANDBOX_SETTINGSPAGE_H

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/Button.h"
#include "HUD/CoordsConverter.h"

class SettingsPage : public Page {
public:
    void Init() override;
    void Update() override;

    const char* _backgroundMainMenuImagePath = "res/Images/MainMenuSettings/MainMenuSettingsBackground.png";
    ImageRenderer _backgroundMainMenu;

    std::shared_ptr<Button> _musicMinusButton;
    std::shared_ptr<Button> _musicPlusButton;
    std::shared_ptr<Button> _sfxMinusButton;
    std::shared_ptr<Button> _sfxPlusButton;
    std::shared_ptr<Button> _discoButtonOff;
    std::shared_ptr<Button> _discoButtonOn;
    std::shared_ptr<Button> _controlsButton;
    std::shared_ptr<Button> _backButton;

    ImageRenderer _buttonsBackground;
    ImageRenderer _logo;

    bool _isDisco = true;

    int _mode = 0; //0 - settings from main menu, 1 - settings from pause menu

    void ChangeModeTo(int mode);

private:
    void SetupButtons();
};


#endif //SANDBOX_SETTINGSPAGE_H
