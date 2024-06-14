#pragma once

#include "HUD/Pages/Page.h"

class PauseMenuPage : public Page {

public:
    const char* _backgroundImagePath = "res/Images/pauseBackground.png";

    ImageRenderer _logo;
    const char* _logoImagePath = "res/Images/logo.png";

    void Init() override;

    void Update() override;
};

