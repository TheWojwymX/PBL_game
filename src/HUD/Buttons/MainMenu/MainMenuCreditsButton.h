//
// Created by Jacek on 10.06.2024.
//

#ifndef SANDBOX_MAINMENUCREDITSBUTTON_H
#define SANDBOX_MAINMENUCREDITSBUTTON_H

#include "HUD/Buttons/Button.h"

class MainMenuCreditsButton : public Button {
public:
    void Init() override;

    void Update() override;

    void Onclick() override;

    glm::vec3 leftDownCorner = glm::vec3(-0.1875, -0.4574074, 0.0);
    glm::vec3 rightTopCorner = glm::vec3(0.1859375, -0.2722222, 0.0);
    array<float, 32> _buttonVertices{
            // positions          // colors           // texture coords
            rightTopCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            rightTopCorner.x, rightTopCorner.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            leftDownCorner.x, rightTopCorner.y, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            leftDownCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    std::string _text = "";

    void SetVertices(const array<float, 32> &vertices) override;

    void SetText(std::string text, float textSize) override;

    const char *_backgroundImagePath = "res/Images/Button/credits.png";
    const char *_hoverBackgroundImagePath = "res/Images/Button/credits_hover.png";
    const char *_clickedBackgroundImagePath = "res/Images/Button/credits_clicked.png";

};

#endif //SANDBOX_MAINMENUCREDITSBUTTON_H
