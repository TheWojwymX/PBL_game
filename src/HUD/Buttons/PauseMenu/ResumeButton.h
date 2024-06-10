//
// Created by Jacek on 09.05.2024.
//

#ifndef SANDBOX_RESUMEBUTTON_H
#define SANDBOX_RESUMEBUTTON_H


#include "HUD/Buttons/Button.h"

class ResumeButton : public Button {
public:

    void Init() override;

    void Update() override;

    void Onclick() override;

    std::string _text = "";

    glm::vec3 leftDownCorner = glm::vec3(-0.1864583, 0.1296296, 0.0);
    glm::vec3 rightTopCorner = glm::vec3(0.1875, 0.3166667, 0.0);
    array<float, 32> _buttonVertices{
            // positions          // colors           // texture coords
            rightTopCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            rightTopCorner.x, rightTopCorner.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            leftDownCorner.x, rightTopCorner.y, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            leftDownCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    void SetVertices(const array<float, 32> &vertices) override;

    void SetText(std::string text, float textSize) override;

    const char *_backgroundImagePath = "res/Images/Button/resume.png";
    const char *_hoverBackgroundImagePath = "res/Images/Button/resume_hover.png";
    const char *_clickedBackgroundImagePath = "res/Images/Button/resume_clicked.png";
};


#endif //SANDBOX_RESUMEBUTTON_H
