//
// Created by Jacek on 22.05.2024.
//

#ifndef SANDBOX_JETPACKREFUEL_H
#define SANDBOX_JETPACKREFUEL_H


#include "HUD/Buttons/Button.h"

class JetpackRefuel : public Button {
public:
    void Init() override;

    void Update() override;

    void Onclick() override;

    array<float, 32> _buttonVertices{
            // positions          // colors           // texture coords
            0.3f,  0.40f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.3f, 0.20f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.3f, 0.20f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.3f,  0.40f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    std::string _text = "REFUEL JETPACK";

    void SetVertices(const array<float, 32> &vertices) override;

    void SetText(std::string text, float textSize) override;

};


#endif //SANDBOX_JETPACKREFUEL_H
