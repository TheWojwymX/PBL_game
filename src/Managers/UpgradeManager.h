#pragma once

#include "glm/vec3.hpp"
#include "NodesManager.h"
#include "Core/Node.h"
#include "Core/Input.h"

#define UPGRADEMANAGER UpgradeManager::getInstance()

class UpgradeManager {
public:
    float _radiusSquared = 25.0f;

    static UpgradeManager &getInstance();
    UpgradeManager() = default;
    ~UpgradeManager() = default;
    UpgradeManager(const UpgradeManager &) = delete;
    UpgradeManager &operator=(const UpgradeManager &) = delete;
<<<<<<< Updated upstream
    bool checkActivation();

private:
    bool isDomeStationInRange();
=======

    bool RayIntersectsBoundingBox(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
                                  const glm::vec3& minBoundingBox, const glm::vec3& maxBoundingBox);

    void upgradeDomeHp();
    void upgradeTurretDamage();
    void upgradeTurretFireRate();

    bool isDomeStationInRaycast();
    bool isDomeStationInRange();

    bool isTurretInRaycast();
    bool isTurretInRange();
>>>>>>> Stashed changes

    bool isPlayerStationInRaycast();
    bool isPlayerStationInRange();



};


