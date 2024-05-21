#pragma once

#include "glm/vec3.hpp"
#include "NodesManager.h"
#include "Core/Node.h"
#include "Core/Input.h"

#define UPGRADEMANAGER UpgradeManager::getInstance()

class UpgradeManager {
public:
    glm::vec3 _domeStation = {40,100,90};
    float _radiusSquared = 25.0f;

    static UpgradeManager &getInstance();
    UpgradeManager() = default;
    ~UpgradeManager() = default;
    UpgradeManager(const UpgradeManager &) = delete;
    UpgradeManager &operator=(const UpgradeManager &) = delete;
    bool checkActivation();

private:
    bool isDomeStationInRange();

};

