#pragma once

#include "Core/Component.h"
#include "Core/ComponentTypeEnum.h"
#include "Managers/GameManager.h"
#include <regex>

enum EnemyType { ANT, BEETLE, WASP };

class Enemy : public Component, public std::enable_shared_from_this<Enemy> {

public:
    bool _isAvoiding = false;
    bool _isAtWalls = false;
    bool _shouldStay = false;

    glm::vec3 _destinationVector;
    EnemyType _enemyType = ANT;
    float _speed = 5;
    float _hp = 100;
    float _damage = 5;
    float _attackFrequency = 3;
    float _size = 2;


    float _distanceToStop = 0.1;
    float _slalomTime = 0.0f;
    float _attackTimer = _attackFrequency;
    float _slalomAmplitude = 1.0f;
    float _slalomFrequency = 1.0f;

    Enemy();
    nlohmann::json Serialize() override;
    void Deserialize(const nlohmann::json &jsonData) override;

    void WalkToDestination(glm::vec3 *destination = nullptr);
    void TakeDamage(float amount);
    void Die();
    void AttackDome();
    void EnemyAI();
    void SetStats(EnemyType type, float speed, float hp, float damage, float attackFrequency, float size);
private:
    std::vector<std::shared_ptr<Sound>> _attackSounds;

    void InitSounds();
};
