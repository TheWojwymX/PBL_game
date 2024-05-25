//
// Created by Jacek on 09.05.2024.
//

#ifndef SANDBOX_BUTTON_H
#define SANDBOX_BUTTON_H

#include "../Image/ImageRenderer.h"
#include "../Text/TextRenderer.h"
#include "Core/Input.h"

class Button {
public:

    const char *_backgroundImagePath = "../../res/Images/Button/idle.png";
    const char *_hoverBackgroundImagePath = "../../res/Images/Button/hover.png";
    const char *_clickedBackgroundImagePath = "../../res/Images/Button/clicked.png";

    ImageRenderer _backgroundImage;
    ImageRenderer _hoverBackgroundImage;
    ImageRenderer _clickedBackgroundImage;

    std::string _text = "default";

    virtual void Init();
    virtual void Update();
    void CheckClick();
    bool CheckHover();

    virtual void Onclick();

    virtual void SetVertices(const array<float, 32> &vertices);

    virtual void SetText(std::string text);

protected:

    float _clickedTimer = 0.0f;
    bool _timerOn = false;

    array<float, 32> _buttonVertices{
            // positions          // colors           // texture coords
            0.03f,  0.05f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.03f, -0.05f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.03f, -0.05f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.03f,  0.05f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    void AppareanceManager();

    void TimerCount();
};


#endif //SANDBOX_BUTTON_H
