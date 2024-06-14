#pragma once

#include "HUD/Buttons/Button.h"

class MainMenuStartButton : public Button {
public:
    void Init() override;

    void Update() override;

    void Onclick() override;

    std::string _text = "";

    void SetText(std::string text, float textSize) override;

    const char *_backgroundImagePath = "res/Images/Button/start.png";
    const char *_hoverBackgroundImagePath = "res/Images/Button/start_hover.png";
    const char *_clickedBackgroundImagePath = "res/Images/Button/start_clicked.png";

};