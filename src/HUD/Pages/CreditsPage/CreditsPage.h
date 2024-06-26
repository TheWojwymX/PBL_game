//
// Created by Jacek on 26.06.2024.
//

#ifndef SANDBOX_CREDITSPAGE_H
#define SANDBOX_CREDITSPAGE_H

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/Button.h"
#include "HUD/CoordsConverter.h"

class CreditsPage : public Page {
public:
    void Init() override;
    void Update() override;

    const char* _backgroundImagePath = "res/Images/MainMenuSettings/MainMenuSettingsBackground.png";

    std::shared_ptr<Button> _upButton;
    std::shared_ptr<Button> _downButton;
    std::shared_ptr<Button> _upButtonBlocked;
    std::shared_ptr<Button> _downButtonBlocked;

    std::shared_ptr<Button> _backButton;

    ImageRenderer _credit;
    std::vector<shared_ptr<ImageRenderer>> _credits;
    int _pageIndex = 0;

private:
    void SetupButtons();
};


#endif //SANDBOX_CREDITSPAGE_H
