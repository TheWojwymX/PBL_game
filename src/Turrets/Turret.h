#pragma once

#include "Core/Component.h"

enum TurretType { MINIGUN, SNIPER, RIFLE };

class Turret : public Component, public std::enable_shared_from_this<Turret> {

public:
    Turret();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void Update() override;

    void setUp();
    void Upgrade(glm::vec2 values);

    void SetTurretType(TurretType type) { _turretType = type; }
    TurretType GetTurretType() { return _turretType; }
    char GetUpgradeLevel() { return _upgradeLevel; }


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

    std::shared_ptr<Node> _flare;
private:
    TurretType _turretType = MINIGUN;
    char _upgradeLevel = 0;

    void HandleSpawn();
    void UpdateModel();
};
