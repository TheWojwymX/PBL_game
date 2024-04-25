//
// Created by Jacek on 20.04.2024.
//

#ifndef SANDBOX_ENEMYAI_H
#define SANDBOX_ENEMYAI_H

#include "Core/Component.h"
#include "Core/ComponentTypeEnum.h"

class EnemyAI : public Component {

public:

    EnemyAI();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void Update() override;

    glm::vec3 _destinationVector;

    float _distanceToStop;

    bool _isFlying = false;

    void WalkToDestination(glm::vec3 *destination = nullptr);

    int _walkingSpeed = 5;

    int _enemySize;

    bool _isAvoiding = false;
};


#endif //SANDBOX_ENEMYAI_H
