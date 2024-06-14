#pragma once

#include "HUD/Buttons/Button.h"

class JetpackRefuel : public Button {
public:
    void Init() override;

    void Update() override;

    void Onclick() override;

    void SetText(std::string text, float textSize) override;

};