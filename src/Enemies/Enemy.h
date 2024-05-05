//
// Created by Jacek on 20.04.2024.
//

#ifndef SANDBOX_ENEMY_H
#define SANDBOX_ENEMY_H

#include "Core/Component.h"
#include "Core/ComponentTypeEnum.h"
#include "Managers/GameManager.h"

class Enemy : public Component {

public:

    Enemy();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void Update() override;

    glm::vec3 _destinationVector;

    float _distanceToStop = 0.1;

    bool _isFlying = false;

    void WalkToDestination(glm::vec3 *destination = nullptr);

    int _walkingSpeed = 5;

    int _enemySize;

    bool _isAvoiding = false;

    float slalomTime = 0.0f;

    bool _isAtWalls = false;

    bool _shouldStay = false;

    int _hp = 100;
};

#endif //SANDBOX_ENEMY_H
