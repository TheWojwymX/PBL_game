#pragma once

#include "HUD/Pages/Page.h"
#include "HUD/Text/TextRenderer.h"

class MessagePage : public Page  {
public:
    const char *_backgroundImagePath = "res/Images/HUD/tutorial_window.png";
    ImageRenderer _backgroundImage;

    void Init() override;

    void Update() override;
    string _actualText = "default";
};
