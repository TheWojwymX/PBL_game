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
    glm::vec3 destinationVectorWalking = glm::vec3 (GAMEMANAGER._mapSizeX/2, GAMEMANAGER._groundPosition, GAMEMANAGER._mapSizeZ/2);
    glm::vec3 destinationVectorFlying = glm::vec3 (GAMEMANAGER._mapSizeX/2, GAMEMANAGER._groundPosition + GAMEMANAGER._domeRadius/2, GAMEMANAGER._mapSizeZ/2);
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
}

void EnemiesAIManager::Update() {
    for(int i = 0; i < _enemiesAI.size(); i++){

    }
}
