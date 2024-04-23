//
// Created by Jacek on 20.04.2024.
//

#include "EnemyAI.h"
#include "Core/Time.h"

EnemyAI::EnemyAI() {
    _type = ComponentType::ENEMYAI;
}

nlohmann::json EnemyAI::Serialize() {
    return Component::Serialize();
}

void EnemyAI::Deserialize(const nlohmann::json &jsonData) {
    Component::Deserialize(jsonData);
}

float slalomAmplitude = 0.02f; // amplituda ruchu bocznego
float slalomFrequency = 2.0f; // częstość ruchu bocznego (ile razy na jednostkę odległości)
float slalomTime = 0.0f; // zmienna śledząca upływ czasu lub odległość

void EnemyAI::WalkToDome(){

    float ourX = _ownerTransform->GetPosition().x;
    float ourY = _ownerTransform->GetPosition().y;
    float ourZ = _ownerTransform->GetPosition().z;

    float domeX = _destinationVector.x;
    float domeY = _destinationVector.y;
    float domeZ = _destinationVector.z;

    if(hypot(hypot(ourX-domeX,ourY-domeY),ourZ-domeZ) > _distanceToStop){

        slalomTime += TIME.GetDeltaTime();
        float sideMovement = sin(slalomTime * slalomFrequency) * slalomAmplitude;
        glm::vec3 sideVector = glm::normalize(glm::cross(_ownerTransform->GetRotation() * glm::vec3(0, 1, 0), _ownerTransform->GetRotation() * glm::vec3(0, 0, -1))) * sideMovement;
        glm::vec3 forwardMovement = Transform::MoveTowards(_ownerTransform->GetPosition(), _destinationVector, 0.01);
        //std::cout << sideVector.x << ", " << sideVector.y << ", " << sideVector.z  << std::endl;
        _ownerTransform->LookAtPosition(forwardMovement + sideVector);
        _ownerTransform->SetPosition(forwardMovement + sideVector);
    }
}

void EnemyAI::Update() {
    WalkToDome();
    Component::Update();
}
