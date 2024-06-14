#pragma once

#include "HUD/Buttons/Button.h"
#include "Managers/UpgradeManager.h"

class UpgradeDomeHP : public Button {
public:
    void Init() override;

    void Update() override;

    void Onclick() override;
    std::string _text = "UPGRADE MAX HP";

    void SetText(std::string text, float textSize) override;

};