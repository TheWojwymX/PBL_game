#pragma once

#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "NodesManager.h"
#include "../Turrets/TurretsManager.h"
#include "DomeManager.h"
#include "Core/Node.h"
#include "Core/Input.h"

#define UPGRADEMANAGER UpgradeManager::GetInstance()

struct Upgrades {
    std::vector<glm::ivec2> upgradeCosts;
    std::vector<float> upgradeValues;
};

class UpgradeManager {
public:
    glm::vec3 _domeStation = { 40, 100, 90 };
    float _radiusSquared = 25.0f;

    static UpgradeManager& GetInstance();
    UpgradeManager();
    ~UpgradeManager() = default;
    UpgradeManager(const UpgradeManager&) = delete;
    UpgradeManager& operator=(const UpgradeManager&) = delete;

    void HighlightSingleTurret(int turretIndex);

    bool RayIntersectsBoundingBox(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
        const glm::vec3& minBoundingBox, const glm::vec3& maxBoundingBox);

    //Base upgrade station
    void UpgradeDomeHp();
    void UpgradeDomeHPRegen();
    void DomeHPRepair();
    void Evacuate();

    //Player upgrade station Guziki do upgradu:   Jetpack Capacity, Mining reach, Mining Radius, Mining Speed
    void UpgradeJetpackCapacity();
    void UpgradeMiningSpeed();
    void UpgradeMiningReach();
    void UpgradeMiningRadius();

    void UpgradeTurretDamage();
    void UpgradeTurretFireRate();

    bool IsDomeStationInRaycast();
    bool IsDomeStationInRange();

    bool IsTurretInRaycast();
    bool IsTurretInRange();

    bool IsPlayerStationInRaycast();
    bool IsPlayerStationInRange();

    void Update();

    int _turretFireRateUpgradeCost = 10;
    int _turretDamageUpgradeCost = 10;

private:
    Upgrades _domeHPUpgrades;
    Upgrades _domeHPRegenUpgrades;
    Upgrades _domeHPRepair;
    Upgrades _evacuateCost;

    Upgrades _jetpackCapacityUpgrades;
    Upgrades _miningSpeedUpgrades;
    Upgrades _miningReachUpgrades;
    Upgrades _miningRadiusUpgrades;

    std::shared_ptr<PlayerController> _playerRef;
};