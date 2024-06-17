#include "GlowstickMovement.h"

GlowstickMovement::GlowstickMovement() : _movementVector(glm::vec3(0.0f)), _velocity(glm::vec3(0.0f))
{
    _type = ComponentType::GLOWSTICKMOVEMENT;
}

GlowstickMovement::~GlowstickMovement() {
    _velocity = glm::vec3(0.0f,0.0f,0.0f);
    _frontVector = glm::vec3(0.0f,0.0f,0.0f);
    _movementVector = glm::vec3(0.0f,0.0f,0.0f);
    _blockManagerRef->Reset();
}

void GlowstickMovement::Init() {
    _blockManagerRef = COMPONENTSMANAGER.GetComponentByID<BlockManager>(1);
    _frontVector = COMPONENTSMANAGER.GetComponentByID<Camera>(2)->GetFrontVector();
    float initialSpeed = 15.0f;
    _velocity = _frontVector * initialSpeed;
}

void GlowstickMovement::Update() {
    _velocity.y -= 9.8f * TIME.GetDeltaTime();

    _movementVector = _velocity * TIME.GetDeltaTime();

    _ownerTransform->AddPosition(_movementVector);

    std::tuple<bool, BlockData*, glm::vec3> collisionResult = _blockManagerRef->CheckSimpleEntityCollision(_ownerTransform->GetPosition());

    if (std::get<0>(collisionResult))
    {
        _ownerTransform->AddPosition(std::get<2>(collisionResult));
        ResolveCollision(std::get<1>(collisionResult));
    }
}

void GlowstickMovement::ResumeFall()
{
    _velocity = glm::vec3(0.0f);
    SetEnabled(true);
}

void GlowstickMovement::ResolveCollision(BlockData* block)
{
    block->AddGlowstick(shared_from_this());
    SetEnabled(false);
}
