//
// Created by Jacek on 20.04.2024.
//

#include "EnemiesAIManager.h"

EnemiesAIManager &EnemiesAIManager::getInstance() {
    static EnemiesAIManager instance;
    return instance;
}

void EnemiesAIManager::Init() {
    for(int i = 0; i < COMPONENTSMANAGER.GetComponents().size(); i++){
        if(COMPONENTSMANAGER.GetComponents()[i] != nullptr && COMPONENTSMANAGER.GetComponents()[i]->_type == ComponentType::ENEMYAI){
            _enemiesAI.push_back(std::dynamic_pointer_cast<EnemyAI>(COMPONENTSMANAGER.GetComponents()[i]));
        }
    }

    //set destinations
    destinationVectorWalking = glm::vec3 (GAMEMANAGER._mapSizeX/2, GAMEMANAGER._groundPosition, GAMEMANAGER._mapSizeZ/2);
    destinationVectorFlying = glm::vec3 (GAMEMANAGER._mapSizeX/2, GAMEMANAGER._groundPosition + GAMEMANAGER._domeRadius/2, GAMEMANAGER._mapSizeZ/2);
    for(int i = 0; i < _enemiesAI.size(); i++){
        if(_enemiesAI[i]->_isFlying){
            _enemiesAI[i]->_destinationVector = destinationVectorFlying;
        }
        else{
            _enemiesAI[i]->_destinationVector = destinationVectorWalking;
        }
    }

    float distanceToStop = GAMEMANAGER._domeRadius;

    //set distance when should stop
    for(int i = 0; i < _enemiesAI.size(); i++){
        _enemiesAI[i]->_distanceToStop = distanceToStop;
    }

    //set enemy size
    for(int i = 0; i < _enemiesAI.size(); i++){
        _enemiesAI[i]->_enemySize = _enemiesSize;
    }
}

void EnemiesAIManager::CheckNormalDestination(){
    for(int i = 0; i < _enemiesAI.size(); i++){
        if(_enemiesAI[i]->_destinationVector != destinationVectorWalking && _enemiesAI[i]->_destinationVector != destinationVectorFlying
        && glm::distance(_enemiesAI[i]->_destinationVector, _enemiesAI[i]->GetOwnerPosition()) <= 1){
            if(_enemiesAI[i]->_isFlying){
                _enemiesAI[i]->_destinationVector = destinationVectorFlying;
            }
            else{
                _enemiesAI[i]->_destinationVector = destinationVectorWalking;
            }

            _enemiesAI[i]->_isAvoiding = false;
        }
    }
}

void EnemiesAIManager::CheckDistance(){
    for(int i = 0; i < _enemiesAI.size(); i++) {
        if(_enemiesAI[i]->_isAvoiding){
            continue;
        }

        glm::vec3 avoidanceVector(0.0f);
        bool needsToAvoid = false;

        for(int j = 0; j < _enemiesAI.size(); j++) {
            if (i == j) continue;

            if(_enemiesAI[j]->_isAvoiding){
                continue;
            }

            glm::vec3 toOther = _enemiesAI[j]->GetOwnerPosition() - _enemiesAI[i]->GetOwnerPosition();
            float distanceToOther = glm::length(toOther);

            if (distanceToOther < _enemiesSize) {
                needsToAvoid = true;
                toOther = glm::normalize(toOther);
                avoidanceVector -= toOther / distanceToOther;
            }
        }

        if (needsToAvoid) {
            avoidanceVector = glm::normalize(avoidanceVector);
            _enemiesAI[i]->_isAvoiding = true;
            _enemiesAI[i]->_destinationVector = _enemiesAI[i]->GetOwnerPosition() + avoidanceVector * 3.0f;
            _enemiesAI[i]->_distanceToStop = 1;
        } else {
            _enemiesAI[i]->_isAvoiding = false;
        }
    }
}

void EnemiesAIManager::Update() {

    CheckNormalDestination();
    CheckDistance();

}
