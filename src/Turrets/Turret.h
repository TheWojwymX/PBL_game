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

    float _swingTimer = 0.0f;
    int _swingDirection = 1;
    float _rightMax = 1;
    float _leftMax = -1;

    glm::quat _finalRotation;
    glm::vec3 _finalPosition;
};


#endif //SANDBOX_TURRET_H
