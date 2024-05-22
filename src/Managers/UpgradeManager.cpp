//
// Created by TheWojwymX on 13.05.2024.
//

#include "UpgradeManager.h"

UpgradeManager &UpgradeManager::getInstance() {
    static UpgradeManager instance;
    return instance;
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

bool UpgradeManager::isDomeStationInRaycast() {
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
<<<<<<< Updated upstream
            cout << "pressed";
=======
>>>>>>> Stashed changes
            return true;
        }
    }
    return false;
}

bool UpgradeManager::isDomeStationInRange()
{

    auto _domeStation = NODESMANAGER.getNodeByName("DomeStation")->GetTransform()->GetPosition();
    auto playerPosition = NODESMANAGER.getNodeByName("player")->GetTransform()->GetPosition();
    float distanceX = (_domeStation.x - playerPosition.x) * (_domeStation.x - playerPosition.x);
    float distanceY = (_domeStation.y - playerPosition.y) * (_domeStation.y - playerPosition.y);
    float distanceZ = (_domeStation.z - playerPosition.z) * (_domeStation.z - playerPosition.z);
    float distance = distanceX + distanceY + distanceZ;

    return distance + 0.01 <= _radiusSquared;
}
<<<<<<< Updated upstream
=======

bool UpgradeManager::isPlayerStationInRaycast()
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
            std::cout << "work" << endl;
            return true;
        }
    }
    return false;
}

bool UpgradeManager::isPlayerStationInRange()
{
    auto _playerStation = NODESMANAGER.getNodeByName("PlayerStation")->GetTransform()->GetPosition();
    auto playerPosition = NODESMANAGER.getNodeByName("player")->GetTransform()->GetPosition();
    float distanceX = (_playerStation.x - playerPosition.x) * (_playerStation.x - playerPosition.x);
    float distanceY = (_playerStation.y - playerPosition.y) * (_playerStation.y - playerPosition.y);
    float distanceZ = (_playerStation.z - playerPosition.z) * (_playerStation.z - playerPosition.z);
    float distance = distanceX + distanceY + distanceZ;

    return distance + 0.01 <= _radiusSquared;
}

bool UpgradeManager::isTurretInRaycast()
{
    if(TURRETSMANAGER.RaycastTurrets() != -1)
    {
        return true;
    }
    return false;
}

bool UpgradeManager::isTurretInRange()
{
    if(TURRETSMANAGER.isSelectedTurretInRange())
    {
        return true;
    }
    return false;
}

void UpgradeManager::upgradeDomeHp()
{
    DOMEMANAGER.maxHP += 20;
    cout << "Current dome max HP: " << DOMEMANAGER.maxHP << endl;
}

void UpgradeManager::upgradeTurretDamage()
{
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

void UpgradeManager::upgradeTurretFireRate() {
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


>>>>>>> Stashed changes
