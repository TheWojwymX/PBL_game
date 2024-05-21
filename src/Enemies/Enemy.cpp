//
// Created by Jacek on 20.04.2024.
//

#include "Enemy.h"
#include "Core/Time.h"
#include "EnemiesManager.h"
#include "Managers/DomeManager.h"

Enemy::Enemy() {
    _type = ComponentType::ENEMYAI;
}

nlohmann::json Enemy::Serialize() {
    nlohmann::json data;

    data["enemySize"] = _size;

    return data;
}

void Enemy::Deserialize(const nlohmann::json &jsonData) {

    if (jsonData.contains("enemySize")) {
        _size = jsonData["enemySize"].get<float>();
    }

    Component::Deserialize(jsonData);
}

void Enemy::WalkToDestination(glm::vec3 *destination) {

    if(destination != nullptr){
        _destinationVector = *destination;
    }

    float slalomAmplitude = 0.01;
    float slalomFrequency = 1.0f;

    if(glm::distance(_destinationVector, _ownerTransform->GetPosition()) > _distanceToStop && !_shouldStay){

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

void Enemy::Die(){
    ENEMIESMANAGER._testowaPrzeciwnicy++;
    GAMEMANAGER.root->removeChild(_ownerNode);
    auto it = std::find(ENEMIESMANAGER._enemies.begin(), ENEMIESMANAGER._enemies.end(), shared_from_this());
    if (it != ENEMIESMANAGER._enemies.end()) {
        *it = nullptr;
    }
}

void Enemy::TakeDamage(int amount){
    _hp -= amount;
    if(_hp <= 0){
        Die();
    }
}

void Enemy::AttackDome(){

    if(!_isAtWalls) return;

    if(_attackTimer >= _attackFrequency){
        DOMEMANAGER.takeDamage(_damage);
        //std::cout << "DOME HP: " << DOMEMANAGER.hp << std::endl;
        //std::cout << "ATTACKED DOME FOR " << _damage << std::endl;
        _attackTimer = 0;
    }
    else{
        _attackTimer += TIME.GetDeltaTime();
    }
}

//Just like update but connected to EnemiesManager
void Enemy::EnemyAI() {
    AttackDome();
    WalkToDestination();
    Component::Update();
}
