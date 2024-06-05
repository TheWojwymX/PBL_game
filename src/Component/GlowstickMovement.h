#pragma once

#include "Core/Component.h"
#include "Core/Node.h"

class GlowstickMovement : public Component{
public:
    GlowstickMovement();
    void Init() override;
    void Update() override;
private:
    glm::vec3 nodePosition;
    glm::vec3 velocity;
    glm::vec3 frontVector;
};