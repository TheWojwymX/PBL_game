#pragma once

#include "HUD/Buttons/Button.h"

class QuitButton : public Button {
public:
    void Init() override;

    void Update() override;

    void Onclick() override;

    std::string _text = "";

    void SetText(std::string text, float textSize) override;

    const char *_backgroundImagePath = "res/Images/Button/quit.png";
    const char *_hoverBackgroundImagePath = "res/Images/Button/quit_hover.png";
    const char *_clickedBackgroundImagePath = "res/Images/Button/quit_clicked.png";

};