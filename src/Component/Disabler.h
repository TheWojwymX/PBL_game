#pragma once

#include "Core/Component.h"

class Disabler : public Component
{
public:
    Disabler();

    void Deserialize(const nlohmann::json& jsonData) override;
    nlohmann::json Serialize() override;

    void Update() override;

private:
    float _delay;

    void DisableAfterDelay();
};
