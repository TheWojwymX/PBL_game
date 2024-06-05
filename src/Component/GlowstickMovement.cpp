#include "GlowstickMovement.h"

GlowstickMovement::GlowstickMovement() {
    _type = ComponentType::GLOWSTICKMOVEMENT;
}

void GlowstickMovement::Init(){
    frontVector = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetFrontVector();
    float initialSpeed = 5.0f;
    velocity = frontVector * initialSpeed;
}

void GlowstickMovement::Update(){
    nodePosition = _ownerNode->GetTransform()->GetPosition();

    velocity.y -= 9.8f * TIME.GetDeltaTime();

    nodePosition += velocity * TIME.GetDeltaTime();

    _ownerNode->GetTransform()->SetPosition(nodePosition);
}