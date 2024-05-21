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

    std::string _text = "RESUME";

    array<float, 32> _buttonVertices{
            // positions          // colors           // texture coords
            0.3f,  0.4f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.3f, 0.2f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.3f, 0.2f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.3f,  0.4f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    void SetVertices(const array<float, 32> &vertices) override;

    void SetText(std::string text) override;
};


#endif //SANDBOX_RESUMEBUTTON_H
