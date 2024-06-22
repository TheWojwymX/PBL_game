#pragma once

#include "Core/Component.h"

class Disabler : public Component
{
public:
    Disabler();

    void Deserialize(const nlohmann::json& jsonData) override;
    nlohmann::json Serialize() override;

    void Update() override;

    void Reset();

private:
    float _delay;
    float _timer = 0.0f;

    void DisableAfterDelay();
};
