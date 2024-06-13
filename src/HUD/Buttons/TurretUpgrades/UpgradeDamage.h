#pragma once

#include "HUD/Buttons/Button.h"
#include "Managers/UpgradeManager.h"

class UpgradeDamage : public Button {
public:
    void Init() override;

    void Update() override;

    void Onclick() override;

    std::string _text = "UPGRADE DAMAGE";

    void SetText(std::string text, float textSize) override;

};