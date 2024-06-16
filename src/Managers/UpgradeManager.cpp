#include "UpgradeManager.h"

UpgradeManager& UpgradeManager::GetInstance() {
    static UpgradeManager instance;
    return instance;
}

UpgradeManager::UpgradeManager() {
    _playerRef = COMPONENTSMANAGER.GetComponentByID<PlayerController>(3);


    // Dome HP upgrades
    _domeHPUpgrades.upgradeCosts = {
        {10, 20},   //1
        {15, 25},   //2
        {20, 30},   //3
        {25, 35},   //4
        {30, 40}    //5
    };

    _domeHPUpgrades.upgradeValues = {
        20.0f,      //1
        30.0f,      //2
        40.0f,      //3
        50.0f,      //4
        60.0f       //5
    };
    ///////////////////////


    // Dome HP Regen upgrades
    _domeHPRegenUpgrades.upgradeCosts = {
        {8, 15},    //1
        {12, 20},   //2
        {16, 25},   //3
        {20, 30},   //4
        {24, 35}    //5
    };

    _domeHPRegenUpgrades.upgradeValues = {
        1.0f,       //1
        2.0f,       //2
        3.0f,       //3
        4.0f,       //4
        5.0f        //5
    };
    ///////////////////////
    

    // Dome HP Repair upgrade
    _domeHPRepair.upgradeCosts = {
        {5, 5}      // Single cost
    };

    _domeHPRepair.upgradeValues = {
        10.0f       // Single value
    };
    ///////////////////////


    // Evacuate cost
    _evacuateCost.upgradeCosts = {
        {100, 100}  // Single cost
    };
    ///////////////////////


    // JetpackCapacity Upgrades
    _jetpackCapacityUpgrades.upgradeCosts = {
        {10, 20},   // 1
        {15, 25},   // 2
        {20, 30},   // 3
        {25, 35},   // 4
        {30, 40}    // 5
    };

    _jetpackCapacityUpgrades.upgradeValues = {
        50.0f,      // 1
        60.0f,      // 2
        70.0f,      // 3
        80.0f,      // 4
        90.0f       // 5
    };
    ///////////////////////


    // Mining Speed Upgrades
    _miningSpeedUpgrades.upgradeCosts = {
        {15, 25},   // 1
        {20, 30},   // 2
        {25, 35},   // 3
        {30, 40},   // 4
        {35, 45}    // 5
    };

    _miningSpeedUpgrades.upgradeValues = {
        0.25f,       // 1
        0.25f,       // 2
        0.25f,       // 3
        0.25f,       // 4
        0.25f        // 5
    };
    ///////////////////////


    // Mining Reach upgrades
    _miningReachUpgrades.upgradeCosts = {
        {10, 20},   // 1
        {15, 25},   // 2
        {20, 30},   // 3
        {25, 35},   // 4
        {30, 40}    // 5
    };

    _miningReachUpgrades.upgradeValues = {
        2.0f,       // 1
        3.0f,       // 2
        4.0f,       // 3
        5.0f,       // 4
        6.0f        // 5
    };
    ///////////////////////


    // Mining Radius upgrades
    _miningRadiusUpgrades.upgradeCosts = {
        {10, 20},   // 1
        {15, 25},   // 2
        {20, 30},   // 3
        {25, 35},   // 4
        {30, 40}    // 5
    };

    _miningRadiusUpgrades.upgradeValues = {
        1.0f,       // 1
        1.0f,       // 2
        1.0f,       // 3
        1.0f,       // 4
        1.0f        // 5
    };
    ///////////////////////


    // Initialize Turret Upgrades
    // Minigun Upgrades
    _minigunUpgrades.upgradeCosts = {
        {20, 30},   // 1
        {30, 40}    // 2
    };

    _minigunUpgrades.upgradeValues = {
        {1.0f, -0.15f},  // 1 (damage, fire rate)
        {1.0f, -0.25f}   // 2 (damage, fire rate)
    };

    // Sniper Upgrades
    _sniperUpgrades.upgradeCosts = {
        {25, 35},   // 1
        {35, 45}    // 2
    };

    _sniperUpgrades.upgradeValues = {
        {5, -0.25f},  // 1 (damage, fire rate)
        {5, -0.25f}   // 2 (damage, fire rate)
    };

    // Rifle Upgrades
    _rifleUpgrades.upgradeCosts = {
        {15, 25},   // 1
        {25, 35}    // 2
    };

    _rifleUpgrades.upgradeValues = {
        {1.0f, -0.20f},   // 1 (damage, fire rate)
        {1.0f, -0.20f}   // 2 (damage, fire rate)
    };
}

bool UpgradeManager::RayIntersectsBoundingBox(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
    const glm::vec3& minBoundingBox, const glm::vec3& maxBoundingBox) {
    glm::vec3 invDir = 1.0f / rayDirection;
    glm::vec3 tMin = (minBoundingBox - rayOrigin) * invDir;
    glm::vec3 tMax = (maxBoundingBox - rayOrigin) * invDir;

    glm::vec3 tMinSorted = glm::min(tMin, tMax);
    glm::vec3 tMaxSorted = glm::max(tMin, tMax);

    float tEnter = glm::max(glm::max(tMinSorted.x, tMinSorted.y), tMinSorted.z);
    float tExit = glm::min(glm::min(tMaxSorted.x, tMaxSorted.y), tMaxSorted.z);

    // Check for non-intersection or intersection behind ray origin
    if (tExit < 0 || tEnter > tExit)
        return false;

    // Check for intersection
    return true;
}

void UpgradeManager::UpgradeTurret() {
    if (TURRETSMANAGER.selectedIndex == -1) {
        std::cout << "You are not looking at turret" << std::endl;
        return;
    }

    auto selectedTurret = TURRETSMANAGER._turrets[TURRETSMANAGER.selectedIndex];
    TurretType turretType = selectedTurret->GetTurretType();  // Assuming you have a method to get the type
    int upgradeLevel = selectedTurret->GetUpgradeLevel();    // Assuming you have a method to get the current upgrade level

    TurretUpgrades* turretUpgrades;
    switch (turretType) {
    case TurretType::MINIGUN:
        turretUpgrades = &_minigunUpgrades;
        break;
    case TurretType::SNIPER:
        turretUpgrades = &_sniperUpgrades;
        break;
    case TurretType::RIFLE:
        turretUpgrades = &_rifleUpgrades;
        break;
    default:
        std::cout << "Unknown turret type" << std::endl;
        return;
    }

    // Check if maximum upgrade level is reached
    if (upgradeLevel >= turretUpgrades->upgradeCosts.size()) {
        std::cout << "Max level reached for this turret" << std::endl;
        return;
    }

    glm::ivec2 cost = turretUpgrades->upgradeCosts[upgradeLevel];

    // Check if the player has enough resources
    if (!GAMEMANAGER.HasMaterials(cost)) {
        std::cout << "Not enough resources to upgrade the turret" << std::endl;
        return;
    }

    // Deduct resources
    GAMEMANAGER.RemoveMaterials(cost);

    // Apply the upgrade
    glm::vec2 upgradeValues = turretUpgrades->upgradeValues[upgradeLevel];
    selectedTurret->Upgrade(upgradeValues);
}


bool UpgradeManager::IsDomeStationInRaycast() {
    auto camera = ComponentsManager::getInstance().GetComponentByID<Camera>(2);
    glm::vec3 cameraPosition = camera->GetPosition();
    glm::vec3 rayDirection = camera->GetDirection();

    auto meshRenderer = NODESMANAGER.getNodeByName("DomeStation")->GetComponent<MeshRenderer>();
    if (meshRenderer) {
        auto model = meshRenderer->_model;

        // Transform bounding box to world space
        glm::vec3 minBoundingBox = model->GetMinBoundingBox();
        glm::vec3 maxBoundingBox = model->GetMaxBoundingBox();
        auto transform = NODESMANAGER.getNodeByName("DomeStation")->GetTransform();
        glm::mat4 globalCTM = transform->GetGlobalCTM();
        glm::vec3 transformedMin = glm::vec3(globalCTM * glm::vec4(minBoundingBox, 1.0f));
        glm::vec3 transformedMax = glm::vec3(globalCTM * glm::vec4(maxBoundingBox, 1.0f));

        if (RayIntersectsBoundingBox(cameraPosition, rayDirection, transformedMin, transformedMax))
        {
            return true;
        }
    }
    return false;
}

bool UpgradeManager::IsDomeStationInRange()
{
    auto _domeStation = NODESMANAGER.getNodeByName("DomeStation")->GetTransform()->GetPosition();
    auto playerPosition = NODESMANAGER.getNodeByName("player")->GetTransform()->GetPosition();
    float distance = glm::distance2(_domeStation, playerPosition);

    return distance + 0.01 <= _radiusSquared;
}

bool UpgradeManager::IsPlayerStationInRaycast()
{
    auto camera = ComponentsManager::getInstance().GetComponentByID<Camera>(2);
    glm::vec3 cameraPosition = camera->GetPosition();
    glm::vec3 rayDirection = camera->GetDirection();

    auto meshRenderer = NODESMANAGER.getNodeByName("PlayerStation")->GetComponent<MeshRenderer>();
    if (meshRenderer) {
        auto model = meshRenderer->_model;

        // Transform bounding box to world space
        glm::vec3 minBoundingBox = model->GetMinBoundingBox();
        glm::vec3 maxBoundingBox = model->GetMaxBoundingBox();
        auto transform = NODESMANAGER.getNodeByName("PlayerStation")->GetTransform();
        glm::mat4 globalCTM = transform->GetGlobalCTM();
        glm::vec3 transformedMin = glm::vec3(globalCTM * glm::vec4(minBoundingBox, 1.0f));
        glm::vec3 transformedMax = glm::vec3(globalCTM * glm::vec4(maxBoundingBox, 1.0f));

        if (RayIntersectsBoundingBox(cameraPosition, rayDirection, transformedMin, transformedMax))
        {
            return true;
        }
    }
    return false;
}

bool UpgradeManager::IsPlayerStationInRange()
{
    auto _playerStation = NODESMANAGER.getNodeByName("PlayerStation")->GetTransform()->GetPosition();
    auto playerPosition = NODESMANAGER.getNodeByName("player")->GetTransform()->GetPosition();
    float distance = glm::distance2(_playerStation, playerPosition);

    return distance + 0.01 <= _radiusSquared;
}

bool UpgradeManager::IsTurretInRaycast()
{
    if (TURRETSMANAGER.RaycastTurrets() != -1)
    {
        return true;
    }
    return false;
}

bool UpgradeManager::IsTurretInRange()
{
    if (TURRETSMANAGER.IsSelectedTurretInRange())
    {
        return true;
    }
    return false;
}

void UpgradeManager::UpgradeDomeHp() {
    char domeHPLevel = DOMEMANAGER.GetDomeHPLevel();
    if (domeHPLevel >= _domeHPUpgrades.upgradeCosts.size()) {
        std::cout << "No more upgrades available for Dome HP." << std::endl;
        return;
    }

    // Check if enough resources for upgrade
    glm::ivec2 upgradeCost = _domeHPUpgrades.upgradeCosts[domeHPLevel];
    if (!GAMEMANAGER.HasMaterials(upgradeCost)) {
        std::cout << "Not enough materials to upgrade Dome HP." << std::endl;
        return;
    }

    // Deduct resources
    GAMEMANAGER.RemoveMaterials(upgradeCost);

    // Apply upgrade
    DOMEMANAGER.UpgradeDomeHP(_domeHPUpgrades.upgradeValues[domeHPLevel]);

    // Print current dome max HP
    std::cout << "Current dome max HP: " << DOMEMANAGER.GetDomeMaxHP() << std::endl;
}

void UpgradeManager::UpgradeDomeHPRegen() {
    char domeHPRegenLevel = DOMEMANAGER.GetDomeHPRegenLevel();
    if (domeHPRegenLevel >= _domeHPRegenUpgrades.upgradeCosts.size()) {
        std::cout << "No more upgrades available for Dome HP Regeneration." << std::endl;
        return;
    }

    // Check if enough resources for upgrade
    glm::ivec2 upgradeCost = _domeHPRegenUpgrades.upgradeCosts[domeHPRegenLevel];
    if (!GAMEMANAGER.HasMaterials(upgradeCost)) {
        std::cout << "Not enough materials to upgrade Dome HP Regeneration." << std::endl;
        return;
    }

    // Deduct resources
    GAMEMANAGER.RemoveMaterials(upgradeCost);

    // Apply upgrade
    DOMEMANAGER.UpgradeDomeHPRegen(_domeHPRegenUpgrades.upgradeValues[domeHPRegenLevel]);

    // Print current dome max HP
    std::cout << "Current dome max HP Regen: " << DOMEMANAGER.GetDomeMaxHP() << std::endl;
}

void UpgradeManager::DomeHPRepair()
{
    // Check if enough resources for repair
    glm::ivec2 repairCost = _domeHPRepair.upgradeCosts[0];
    if (!GAMEMANAGER.HasMaterials(repairCost)) {
        std::cout << "Not enough materials to repair Dome HP." << std::endl;
        return;
    }

    // Deduct resources
    GAMEMANAGER.RemoveMaterials(repairCost);

    // Apply repair
    DOMEMANAGER.RepairDomeHP(_domeHPRepair.upgradeValues[0]);

    // Print current dome HP
    std::cout << "Current dome HP after repair: " << DOMEMANAGER.GetDomeHP() << std::endl;
}

void UpgradeManager::Evacuate()
{
    glm::ivec2 evacuateCost = _evacuateCost.upgradeCosts[0];
    if (!GAMEMANAGER.HasMaterials(evacuateCost)) {
        std::cout << "Not enough materials to evacuate." << std::endl;
        return;
    }

    // Deduct resources
    GAMEMANAGER.RemoveMaterials(evacuateCost);

    // Call evacuate
    GAMEMANAGER.Evacuate();
    std::cout << "Evacuation initiated." << std::endl;
}

void UpgradeManager::UpgradeJetpackCapacity()
{
    char jetpackLevel = _playerRef->GetJetpackCapacityLevel();
    if (jetpackLevel >= _jetpackCapacityUpgrades.upgradeCosts.size()) {
        std::cout << "No more upgrades available for Jetpack Capacity." << std::endl;
        return;
    }

    // Check if enough resources for upgrade
    glm::ivec2 upgradeCost = _jetpackCapacityUpgrades.upgradeCosts[jetpackLevel];
    if (!GAMEMANAGER.HasMaterials(upgradeCost)) {
        std::cout << "Not enough materials to upgrade Jetpack Capacity." << std::endl;
        return;
    }

    // Deduct resources
    GAMEMANAGER.RemoveMaterials(upgradeCost);

    // Apply upgrade
    _playerRef->UpgradeJetpackCapacity(_jetpackCapacityUpgrades.upgradeValues[jetpackLevel]);
}

void UpgradeManager::UpgradeMiningSpeed() {
    char miningSpeedLevel = _playerRef->GetMiningSpeedLevel();
    if (miningSpeedLevel >= _miningSpeedUpgrades.upgradeCosts.size()) {
        std::cout << "No more upgrades available for Mining Speed." << std::endl;
        return;
    }

    // Check if enough resources for upgrade
    glm::ivec2 upgradeCost = _miningSpeedUpgrades.upgradeCosts[miningSpeedLevel];
    if (!GAMEMANAGER.HasMaterials(upgradeCost)) {
        std::cout << "Not enough materials to upgrade Mining Speed." << std::endl;
        return;
    }

    // Deduct resources
    GAMEMANAGER.RemoveMaterials(upgradeCost);

    // Apply upgrade
    _playerRef->UpgradeMiningSpeed(_miningSpeedUpgrades.upgradeValues[miningSpeedLevel]);
}

void UpgradeManager::UpgradeMiningReach() {
    char miningReachLevel = _playerRef->GetMiningReachLevel();
    if (miningReachLevel >= _miningReachUpgrades.upgradeCosts.size()) {
        std::cout << "No more upgrades available for Mining Reach." << std::endl;
        return;
    }

    // Check if enough resources for upgrade
    glm::ivec2 upgradeCost = _miningReachUpgrades.upgradeCosts[miningReachLevel];
    if (!GAMEMANAGER.HasMaterials(upgradeCost)) {
        std::cout << "Not enough materials to upgrade Mining Reach." << std::endl;
        return;
    }

    // Deduct resources
    GAMEMANAGER.RemoveMaterials(upgradeCost);

    // Apply upgrade
    _playerRef->UpgradeMiningReach(_miningReachUpgrades.upgradeValues[miningReachLevel]);
}

void UpgradeManager::UpgradeMiningRadius() {
    char miningRadiusLevel = _playerRef->GetMiningRadiusLevel();
    if (miningRadiusLevel >= _miningRadiusUpgrades.upgradeCosts.size()) {
        std::cout << "No more upgrades available for Mining Radius." << std::endl;
        return;
    }

    // Check if enough resources for upgrade
    glm::ivec2 upgradeCost = _miningRadiusUpgrades.upgradeCosts[miningRadiusLevel];
    if (!GAMEMANAGER.HasMaterials(upgradeCost)) {
        std::cout << "Not enough materials to upgrade Mining Radius." << std::endl;
        return;
    }

    // Deduct resources
    GAMEMANAGER.RemoveMaterials(upgradeCost);

    // Apply upgrade
    _playerRef->UpgradeMiningRadius(_miningRadiusUpgrades.upgradeValues[miningRadiusLevel]);
}

void UpgradeManager::Update() {
    if (IsPlayerStationInRaycast() && IsPlayerStationInRange()) {
        NODESMANAGER.getNodeByName("PlayerStation")->GetComponent<MeshRenderer>()->_shouldRenderOutline = true;
    }
    else if (NODESMANAGER.getNodeByName("PlayerStation")->GetComponent<MeshRenderer>()->_shouldRenderOutline == true) {
        NODESMANAGER.getNodeByName("PlayerStation")->GetComponent<MeshRenderer>()->_shouldRenderOutline = false;
    }

    if (IsDomeStationInRaycast() && IsDomeStationInRange()) {
        NODESMANAGER.getNodeByName("DomeStation")->GetComponent<MeshRenderer>()->_shouldRenderOutline = true;
    }
    else if (NODESMANAGER.getNodeByName("DomeStation")->GetComponent<MeshRenderer>()->_shouldRenderOutline == true) {
        NODESMANAGER.getNodeByName("DomeStation")->GetComponent<MeshRenderer>()->_shouldRenderOutline = false;
    }

    if (IsTurretInRaycast() && IsTurretInRange()) {
        HighlightSingleTurret(TURRETSMANAGER.RaycastTurrets());

        if (Input::Instance().IsKeyPressed(GLFW_KEY_E)) {
            UpgradeTurret();
        }

        if(timeLooking < 0.5f){
            timeLooking += TIME.GetDeltaTime();
        }
    }
    else {
        if(timeLooking > 0.0f) timeLooking -= TIME.GetDeltaTime() * 1.2f;
        for (int i = 0; i < TURRETSMANAGER._turrets.size(); i++) {
            TURRETSMANAGER._turrets[i]->_ownerNode->GetComponent<MeshRenderer>()->_shouldRenderOutline = false;
            auto particleGenerators = TURRETSMANAGER._turrets[i]->_ownerNode->GetAllComponents<ParticleGenerator>();
            for (const auto &generator: particleGenerators) {
                if (generator->particleType == "tooltipParticle") {
                    if(timeLooking < 0.5f) generator->tooltipShrink = true;
                    if(timeLooking <= 0.0f) {
                        generator->tooltipSpawn = false;
                        timeLooking = 0.0f;
                    }
                }
            }
        }
    }
}

void UpgradeManager::HighlightSingleTurret(int turretIndex)
{
    std::shared_ptr<ParticleGenerator> particle;
    auto particleGenerators = TURRETSMANAGER._turrets[turretIndex]->_ownerNode->GetAllComponents<ParticleGenerator>();
    for (const auto &generator: particleGenerators) {
        if (generator->particleType == "tooltipParticle") {
            particle = generator;
        }
    }

    for (int i = 0; i < TURRETSMANAGER._turrets.size(); i++) {
        TURRETSMANAGER._turrets[i]->_ownerNode->GetComponent<MeshRenderer>()->_shouldRenderOutline = false;
        if(particle != nullptr) {
            if(timeLooking < 0.5f) particle->tooltipShrink = true;
            if(timeLooking <= 0.0f) {
                particle->tooltipSpawn = false;
                timeLooking = 0.0f;
            }
        }
    }

    // Highlight the specified turret
    if (turretIndex != -1) {
        TURRETSMANAGER._turrets[turretIndex]->_ownerNode->GetComponent<MeshRenderer>()->_shouldRenderOutline = true;
        if(particle != nullptr && timeLooking > 0.5f) {
            particle->tooltipSpawn = true;
            particle->tooltipShrink = false;
            particle->SpawnParticles();
        }
    }
}