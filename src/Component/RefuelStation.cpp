//
// Created by Jacek on 24.05.2024.
//

#include "RefuelStation.h"


RefuelStation::RefuelStation() {
    _type = ComponentType::REFUELSTATION;
}

nlohmann::json RefuelStation::Serialize() {
    return Component::Serialize();
}

void RefuelStation::Deserialize(const nlohmann::json &jsonData) {
    Component::Deserialize(jsonData);
}

void RefuelStation::Init() {
    _playerNode = NODESMANAGER.getNodeByName("player");
    Component::Init();
}

void RefuelStation::Update() {
    if(isPlayerInRange() && _prepareTimer < _timeToStartFuel){
        _prepareTimer += TIME.GetDeltaTime();
    }
    else if(isPlayerInRange() && _prepareTimer >= _timeToStartFuel){
        Refuel();
    }
    else if(!isPlayerInRange() && _prepareTimer != 0){
        _prepareTimer = 0;
    }


    Component::Update();
}

void RefuelStation::Refuel(){

    auto playerController = _playerNode->GetComponent<PlayerController>();

    if(playerController->_jetpackFuel < playerController->_maxJetpackFuel){
        if(_fuelTimer < _chargeInterval){
            _fuelTimer += TIME.GetDeltaTime();
        }
        else{
            //std::cout << "Your fuel is " << playerController->_jetpackFuel << std::endl;
            _fuelTimer = 0;
            playerController->_jetpackFuel += _howMuchFuelPerCharge;
        }
    }
    else{
        playerController->_jetpackFuel = playerController->_maxJetpackFuel;
        //std::cout << "You already have max fuel!" << std::endl;
    }
}

bool RefuelStation::isPlayerInRange()
{
    auto refuelStationPosition = this->_ownerNode->GetTransform()->GetPosition();
    auto playerPosition = _playerNode->GetTransform()->GetPosition();

    return glm::distance(refuelStationPosition, playerPosition) <= _refuelRadius;
}