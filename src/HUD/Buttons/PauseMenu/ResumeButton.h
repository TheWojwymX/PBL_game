#pragma once

#include "HUD/Buttons/Button.h"

class ResumeButton : public Button {
public:

    void Init() override;

    void Update() override;

    void Onclick() override;

    std::string _text = "";

    void SetText(std::string text, float textSize) override;

    const char *_backgroundImagePath = "res/Images/Button/resume.png";
    const char *_hoverBackgroundImagePath = "res/Images/Button/resume_hover.png";
    const char *_clickedBackgroundImagePath = "res/Images/Button/resume_clicked.png";
};
