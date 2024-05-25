//
// Created by Jacek on 21.05.2024.
//

#ifndef SANDBOX_UPGRADEDOMEHP_H
#define SANDBOX_UPGRADEDOMEHP_H


#include "HUD/Buttons/Button.h"
#include "Managers/UpgradeManager.h"

class UpgradeDomeHP : public Button {
public:
    void Init() override;

    void Update() override;

    void Onclick() override;

    array<float, 32> _buttonVertices{
            // positions          // colors           // texture coords
            0.3f, 0.10f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 1.0f, // top right
            0.3f, -0.10f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 0.0f, // bottom right
            -0.3f, -0.10f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, // bottom left
            -0.3f, 0.10f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f  // top left
    };

    std::string _text = "UPGRADE MAX HP";

    void SetVertices(const array<float, 32> &vertices) override;

    void SetText(std::string text, float textSize) override;

};


#endif //SANDBOX_UPGRADEDOMEHP_H
