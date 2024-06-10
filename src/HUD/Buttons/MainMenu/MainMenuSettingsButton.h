//
// Created by Jacek on 10.06.2024.
//

#ifndef SANDBOX_MAINMENUSETTINGSBUTTON_H
#define SANDBOX_MAINMENUSETTINGSBUTTON_H

#include "HUD/Buttons/Button.h"

class MainMenuSettingsButton : public Button {
public:
    void Init() override;

    void Update() override;

    void Onclick() override;

    glm::vec3 leftDownCorner = glm::vec3(-0.1875,-0.2481481, 0.0);
    glm::vec3 rightTopCorner = glm::vec3(0.1859375, -0.0611111, 0.0);
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

    const char *_backgroundImagePath = "res/Images/Button/settings.png";
    const char *_hoverBackgroundImagePath = "res/Images/Button/settings_hover.png";
    const char *_clickedBackgroundImagePath = "res/Images/Button/settings_clicked.png";

};


#endif //SANDBOX_MAINMENUSETTINGSBUTTON_H
