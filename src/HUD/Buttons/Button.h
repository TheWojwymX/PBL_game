#pragma once

#include "../Image/ImageRenderer.h"
#include "../Text/TextRenderer.h"
#include "Core/Input.h"

class Button {
public:

    const char *_backgroundImagePath = "res/Images/Button/idle.png";
    const char *_hoverBackgroundImagePath = "res/Images/Button/hover.png";
    const char *_clickedBackgroundImagePath = "res/Images/Button/clicked.png";

    ImageRenderer _backgroundImage;
    ImageRenderer _hoverBackgroundImage;
    ImageRenderer _clickedBackgroundImage;

    std::string _text = "default";
    float _textSize = 1;

    virtual void Init();
    virtual void Update();
    void CheckClick();
    bool CheckHover();

    virtual void Onclick();

    virtual void SetBackgroundImagePath(const char * path);
    virtual void SetHoverImagePath(const char * path);
    virtual void SetClickedImagePath(const char * path);

    void SetButtonPosition(glm::vec2 position);

    virtual void SetText(std::string text, float textSize);

protected:

    float _clickedTimer = 0.0f;
    bool _timerOn = false;

    void AppareanceManager();

    void TimerCount();

private:
    glm::vec2 _position;
};
