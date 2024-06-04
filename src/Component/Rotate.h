#pragma once

#include "Core/Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>

class Rotate : public Component
{
public:
    Rotate();
    void Deserialize(const nlohmann::json& jsonData) override;
    nlohmann::json Serialize() override;

    void Update() override;

private:
    glm::vec3 _axis;
    float _speed;

    void ApplyRotation();
};
