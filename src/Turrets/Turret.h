#pragma once

#include "Core/Component.h"

enum TurretType { MINIGUN, SNIPER, RIFLE };

class Turret : public Component, public std::enable_shared_from_this<Turret> {

public:
    Turret();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void Update() override;

    void SetUp(glm::vec4 stats);
    void Upgrade(glm::vec4 values);
    void CalculateRangePositions();

    void SetTurretType(TurretType type) { _turretType = type; }
    TurretType GetTurretType() { return _turretType; }
    char GetUpgradeLevel() { return _upgradeLevel; }
    std::pair<int, float> GetSound();
    float GetDamage() { return _damage; }
    float GetFireRate() { return _fireRate; }
    float GetSideRange() { return _sideRange; }
    float GetForwardRange() { return _forwardRange; }

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

    float _timer = 0.0f;
    TurretType _turretType = MINIGUN;
    float _fireRate;
    float _damage;
    float _sideRange;
    float _forwardRange;

    std::shared_ptr<Node> _flare;
private:
    char _upgradeLevel = 0;
    std::vector<std::pair<int, float>> _sounds;

    const char *texture;

    void HandleSpawn();
    void UpdateModel();
    void SetSound();
    void UpdateTooltip();
};
