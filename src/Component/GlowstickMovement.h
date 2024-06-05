#pragma once

#include "Core/Component.h"
#include "Core/Node.h"
#include "Component/BlockManager.h"

class GlowstickMovement : public Component, public std::enable_shared_from_this<GlowstickMovement> {
public:
    GlowstickMovement();
    void Init() override;
    void Update() override;
    void ResumeFall();
private:
    std::shared_ptr<BlockManager> _blockManagerRef;
    glm::vec3 _movementVector;
    glm::vec3 _velocity;
    glm::vec3 _frontVector;

    void ResolveCollision(BlockData* block);
};