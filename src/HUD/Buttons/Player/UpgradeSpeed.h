//
// Created by Jacek on 21.05.2024.
//

#ifndef SANDBOX_UPGRADESPEED_H
#define SANDBOX_UPGRADESPEED_H


#include "HUD/Buttons/Button.h"

class UpgradeSpeed : public Button {
public:
    void Init() override;

    void Update() override;

    void Onclick() override;

    array<float, 32> _buttonVertices{
            // positions          // colors           // texture coords
            0.3f,  -0.20f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.3f, -0.40f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.3f, -0.40f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.3f,  -0.20f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    std::string _text = "UPGRADE SPEED";

    void SetVertices(const array<float, 32> &vertices) override;

    void SetText(std::string text, float textSize) override;

};


#endif //SANDBOX_UPGRADESPEED_H
