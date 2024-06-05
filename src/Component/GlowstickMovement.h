#pragma once

#include "Core/Component.h"
#include "Core/Node.h"
#include "Component/BlockManager.h"

class GlowstickMovement : public Component{
public:
    GlowstickMovement();
    void Init() override;
    void Update() override;
private:
    std::shared_ptr<BlockManager> _blockManagerRef;
    glm::vec3 _movementVector;
    glm::vec3 _velocity;
    glm::vec3 _frontVector;

    void ResolveCollision(glm::vec3 collisionVector);
};