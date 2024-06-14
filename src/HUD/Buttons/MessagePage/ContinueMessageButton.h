#pragma once

#include "HUD/Buttons/Button.h"

class ContinueMessageButton  : public Button {
public:

    void Init() override;

    void Update() override;

    void Onclick() override;

    std::string _text = "CONTINUE";

    void SetText(std::string text, float textSize) override;
};