//
// Created by Jacek on 20.04.2024.
//

#ifndef SANDBOX_ENEMY_H
#define SANDBOX_ENEMY_H

#include "Core/Component.h"
#include "Core/ComponentTypeEnum.h"
#include "Managers/GameManager.h"
#include <regex>

class Enemy : public Component, public std::enable_shared_from_this<Enemy> {

public:

    Enemy();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    glm::vec3 _destinationVector;

    float _distanceToStop = 0.1;

    void WalkToDestination(glm::vec3 *destination = nullptr);

    int _walkingSpeed = 5;

    bool _isAvoiding = false;

    float _slalomTime = 0.0f;

    bool _isAtWalls = false;

    bool _shouldStay = false;

    int _hp = 100;

    int _damage = 5;

    float _attackFrequency = 3;

    float _attackTimer = _attackFrequency;

    float _size = 2;

    float _slalomAmplitude = 0.01;
    float _slalomFrequency = 1.0f;

    void TakeDamage(int amount);

    void Die();

    void AttackDome();

    void EnemyAI();
};

#endif //SANDBOX_ENEMY_H
