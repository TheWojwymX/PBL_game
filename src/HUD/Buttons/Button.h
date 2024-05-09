//
// Created by Jacek on 09.05.2024.
//

#ifndef SANDBOX_BUTTON_H
#define SANDBOX_BUTTON_H

#include "../Image/ImageRenderer.h"
#include "../Text/TextRenderer.h"

class Button {
public:

    array<float, 32> _buttonVertices{
            // positions          // colors           // texture coords
            0.03f,  0.05f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.03f, -0.05f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.03f, -0.05f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.03f,  0.05f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    const char *_backgroundImagePath = "../../../res/Images/ButtonRed.png";
    const char *_hooverBackgroundImagePath = "../../res/Images/ButtonBlue.png";
    const char *_clickedBackgroundImagePath = "../../res/Images/ButtonGreen.png";

    ImageRenderer _backgroundImage;
    ImageRenderer _hooverBackgroundImage;
    ImageRenderer _clickedBackgroundImage;

    std::string _text;
    TextRenderer _textRenderer;

    virtual void Init();
    virtual void Update();
    virtual void OnClick();
    virtual void OnHoover();
    virtual void OnReleaseClick();



};


#endif //SANDBOX_BUTTON_H
