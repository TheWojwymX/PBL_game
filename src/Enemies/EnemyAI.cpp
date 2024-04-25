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

float slalomTime = 0.0f; // zmienna śledząca upływ czasu lub odległość


void EnemyAI::WalkToDestination(glm::vec3 *destination) {

    if(destination != nullptr){
        _destinationVector = *destination;
    }

    float slalomAmplitude = 0.01; // amplituda ruchu bocznego
    float slalomFrequency = 1.0f; // częstość ruchu bocznego (ile razy na jednostkę odległości)

    if(glm::distance(_destinationVector, _ownerTransform->GetPosition()) > _distanceToStop){

        slalomTime += TIME.GetDeltaTime();
        float sideMovement = sin(slalomTime * slalomFrequency) * slalomAmplitude;
        glm::vec3 sideVector = glm::normalize(glm::cross(_ownerTransform->GetRotation() * glm::vec3(0, 1, 0), _ownerTransform->GetRotation() * glm::vec3(0, 0, -1))) * sideMovement;

        glm::vec3 currentPos = _ownerTransform->GetPosition();
        glm::vec3 forwardMovement = Transform::MoveTowards(currentPos, _destinationVector, 0.01f * _walkingSpeed) - currentPos;

        glm::vec3 effectiveMovement = forwardMovement + sideVector;
        glm::vec3 movementDirection = glm::normalize(effectiveMovement);
        movementDirection.y = 0.0f;

        _ownerTransform->LookAt(movementDirection);
        _ownerTransform->SetPosition(currentPos + effectiveMovement);

    }
}

void EnemyAI::Update() {

    WalkToDestination();
    Component::Update();
}