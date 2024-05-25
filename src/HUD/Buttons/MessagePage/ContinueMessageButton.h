//
// Created by Jacek on 25.05.2024.
//

#ifndef SANDBOX_CONTINUEMESSAGEBUTTON_H
#define SANDBOX_CONTINUEMESSAGEBUTTON_H


#include "HUD/Buttons/Button.h"

class ContinueMessageButton  : public Button {
public:

    void Init() override;

    void Update() override;

    void Onclick() override;

    std::string _text = "CONTINUE";

    array<float, 32> _buttonVertices{
            // positions          // colors           // texture coords
            0.9f,  -0.7f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.9f, -0.85f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            0.6f, -0.85f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            0.6f,  -0.7f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    void SetVertices(const array<float, 32> &vertices) override;



    void SetText(std::string text, float textSize) override;
};


#endif //SANDBOX_CONTINUEMESSAGEBUTTON_H
