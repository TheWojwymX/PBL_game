//
// Created by Jacek on 24.05.2024.
//

#ifndef SANDBOX_REFUELSTATION_H
#define SANDBOX_REFUELSTATION_H


#include "Core/Component.h"
#include "Managers/NodesManager.h"

class RefuelStation : public Component {

public:

    RefuelStation();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void Init() override;

    void Update() override;

    std::shared_ptr<Node> _playerNode;

    float _refuelRadius = 5;

    bool isPlayerInRange();

    float _timeToStartFuel = 2;
    float _prepareTimer = 0;

    float _howMuchFuelPerCharge = 5;
    float _chargeInterval = 0.2;
    float _fuelTimer = 0;


    void Refuel();
};


#endif //SANDBOX_REFUELSTATION_H
