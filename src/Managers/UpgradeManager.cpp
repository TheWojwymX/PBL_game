//
// Created by TheWojwymX on 13.05.2024.
//

#include "UpgradeManager.h"

UpgradeManager &UpgradeManager::GetInstance() {
    static UpgradeManager instance;
    return instance;
}

UpgradeManager::UpgradeManager() {
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
    if(TURRETSMANAGER.RaycastTurrets() != -1)
    {
        return true;
    }
    return false;
}

bool UpgradeManager::IsTurretInRange()
{
    if(TURRETSMANAGER.IsSelectedTurretInRange())
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
    std::cout << "Current dome max HP: " << DOMEMANAGER.maxHP << std::endl;
}

void UpgradeManager::UpgradeTurretDamage()
{
    if(GAMEMANAGER._metal < _turretDamageUpgradeCost){
        std::cout << "Brak pieniedzy na ulepszenie obrazen dzialka" << std::endl;
        return;
    }
    else{
        GAMEMANAGER._metal -= _turretDamageUpgradeCost;
    }

    if (TURRETSMANAGER.selectedIndex == -1)
    {
        cout << "You are not looking at turret" << endl;
    }
    else
    {
        TURRETSMANAGER._turrets[TURRETSMANAGER.selectedIndex]->_damage += 5;
        cout << "Current turret damage: " + to_string(TURRETSMANAGER._turrets[TURRETSMANAGER.selectedIndex]->_damage) << endl;
    }
}

void UpgradeManager::UpgradeTurretFireRate() {

    if(GAMEMANAGER._metal < _turretFireRateUpgradeCost){
        std::cout << "Brak pieniedzy na ulepszenie szybkostrzelnosci dzialka" << std::endl;
        return;
    }
    else{
        GAMEMANAGER._metal -= _turretFireRateUpgradeCost;
    }

    if (TURRETSMANAGER.selectedIndex == -1)
    {
        cout << "You are not looking at turret" << endl;
    }
    else
    {
        TURRETSMANAGER._turrets[TURRETSMANAGER.selectedIndex]->_fireRate -= 0.1;
        cout << "Current fire rate: " + to_string(TURRETSMANAGER._turrets[TURRETSMANAGER.selectedIndex]->_fireRate) << endl;
    }
}

void UpgradeManager::Update() {
    if(IsPlayerStationInRaycast() && IsPlayerStationInRange()){
        NODESMANAGER.getNodeByName("PlayerStation")->GetComponent<MeshRenderer>()->_shouldRenderOutline = true;
    }
    else if(NODESMANAGER.getNodeByName("PlayerStation")->GetComponent<MeshRenderer>()->_shouldRenderOutline == true){
        NODESMANAGER.getNodeByName("PlayerStation")->GetComponent<MeshRenderer>()->_shouldRenderOutline = false;
    }

    if(IsDomeStationInRaycast() && IsDomeStationInRange()){
        NODESMANAGER.getNodeByName("DomeStation")->GetComponent<MeshRenderer>()->_shouldRenderOutline = true;
    }
    else if(NODESMANAGER.getNodeByName("DomeStation")->GetComponent<MeshRenderer>()->_shouldRenderOutline == true){
        NODESMANAGER.getNodeByName("DomeStation")->GetComponent<MeshRenderer>()->_shouldRenderOutline = false;
    }

    if(IsTurretInRaycast() && IsTurretInRange()) {
        HighlightSingleTurret(TURRETSMANAGER.RaycastTurrets());
    } else {
        for(int i = 0; i < TURRETSMANAGER._turrets.size(); i++) {
            TURRETSMANAGER._turrets[i]->_ownerNode->GetComponent<MeshRenderer>()->_shouldRenderOutline = false;
        }
    }
}

void UpgradeManager::HighlightSingleTurret(int turretIndex)
{
    for(int i = 0; i < TURRETSMANAGER._turrets.size(); i++) {
        TURRETSMANAGER._turrets[i]->_ownerNode->GetComponent<MeshRenderer>()->_shouldRenderOutline = false;
    }

    // Highlight the specified turret
    if(turretIndex != -1) {
        TURRETSMANAGER._turrets[turretIndex]->_ownerNode->GetComponent<MeshRenderer>()->_shouldRenderOutline = true;
    }
}

