//
// Created by Jacek on 13.05.2024.
//

#ifndef SANDBOX_TURRET_H
#define SANDBOX_TURRET_H


#include "Core/Component.h"

class Turret : public Component, public std::enable_shared_from_this<Turret> {

public:
    Turret();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void Update() override;

    bool _isFlying = false;
    float _flyingSpeed = 2;
    bool _isMoving = false;

    float _swingTimer = 0.0f;
    int _swingDirection = 1;
    float _rightMax = 1;
    float _leftMax = -1;

    glm::quat _finalRotation;
    glm::vec3 _finalPosition;

    std::vector<glm::vec3> _turretRangePositions = {glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0), glm::vec3(0,0,0)};

    float _fireRate = 0.5;
    float _damage = 35;

    float _timer = 0.0f;
};


#endif //SANDBOX_TURRET_H
