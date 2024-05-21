//
// Created by Jacek on 20.04.2024.
//

#include "EnemyAI.h"

EnemyAI::EnemyAI() {
    _type = ComponentType::ENEMYAI;
}

nlohmann::json EnemyAI::Serialize() {
    return Component::Serialize();
}

void EnemyAI::Deserialize(const nlohmann::json &jsonData) {
    Component::Deserialize(jsonData);
}

void EnemyAI::WalkToDome(){

    float ourX = _ownerTransform->GetPosition().x;
    float ourY = _ownerTransform->GetPosition().y;
    float ourZ = _ownerTransform->GetPosition().z;

    float domeX = _destinationVector.x;
    float domeY = _destinationVector.y;
    float domeZ = _destinationVector.z;

    if(hypot(hypot(ourX-domeX,ourY-domeY),ourZ-domeZ) > _distanceToStop){
        _ownerTransform->SetPosition(Transform::MoveTowards(_ownerTransform->GetPosition(), _destinationVector, 0.01));
        _ownerTransform->LookAtPosition(_destinationVector);
    }
}

void EnemyAI::Update() {
    WalkToDome();
    //_ownerNode->GetComponent<ParticleGenerator>()->SpawnParticles();
    Component::Update();
}
