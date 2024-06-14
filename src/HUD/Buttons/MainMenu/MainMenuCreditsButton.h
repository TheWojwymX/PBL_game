#pragma once

#include "HUD/Buttons/Button.h"

class MainMenuCreditsButton : public Button {
public:
    void Init() override;

    void Update() override;

    void Onclick() override;

    std::string _text = "";

    void SetText(std::string text, float textSize) override;

    const char *_backgroundImagePath = "res/Images/Button/credits.png";
    const char *_hoverBackgroundImagePath = "res/Images/Button/credits_hover.png";
    const char *_clickedBackgroundImagePath = "res/Images/Button/credits_clicked.png";

};

