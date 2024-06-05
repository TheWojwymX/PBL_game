#include "GlowstickMovement.h"

GlowstickMovement::GlowstickMovement(): _movementVector(glm::vec3(0.0f)), _velocity(glm::vec3(0.0f))
{
    _type = ComponentType::GLOWSTICKMOVEMENT;
}


void GlowstickMovement::Init(){
    _blockManagerRef = COMPONENTSMANAGER.GetComponentByID<BlockManager>(1);
    _frontVector = COMPONENTSMANAGER.GetComponentByID<Camera>(2)->GetFrontVector();
    float initialSpeed = 13.0f;
    _velocity = _frontVector * initialSpeed;
}

void GlowstickMovement::Update(){
    _velocity.y -= 9.8f * TIME.GetDeltaTime();

    _movementVector = _velocity * TIME.GetDeltaTime();

    std::pair<glm::vec3, glm::vec3> collisionResult = _blockManagerRef->CheckEntityCollision(_ownerTransform->GetPosition(), _movementVector, .05f, .20f);
    _ownerTransform->AddPosition(collisionResult.first);
    
    ResolveCollision(collisionResult.second);
}

void GlowstickMovement::ResolveCollision(glm::vec3 collisionVector)
{
    if (collisionVector != glm::vec3(0.0f))
        SetEnabled(false);
}
