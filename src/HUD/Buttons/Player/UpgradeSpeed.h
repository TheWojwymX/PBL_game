#pragma once

#include "HUD/Buttons/Button.h"

class UpgradeSpeed : public Button {
public:
    void Init() override;

    void Update() override;

    void Onclick() override;

    std::string _text = "UPGRADE SPEED";

    void SetText(std::string text, float textSize) override;

};