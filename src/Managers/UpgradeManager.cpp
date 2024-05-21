//
// Created by TheWojwymX on 13.05.2024.
//

#include "UpgradeManager.h"

UpgradeManager &UpgradeManager::getInstance() {
    static UpgradeManager instance;
    return instance;
}

bool UpgradeManager::checkActivation()
{
    if(INPUT.IsKeyPressed(81))
    {
        if (isDomeStationInRange())
        {
            cout << "pressed upgrade station" << endl;
            return true;
        }
        else if(isTurretInRange())
        {
            cout << "pressed turret" << endl;
            return true;
        }
    }
    return false;
}

bool UpgradeManager::isDomeStationInRange()
{
    glm::vec3 playerPosition = NODESMANAGER.getNodeByName("player")->GetTransform()->GetPosition();
    float distanceX = (_domeStation.x - playerPosition.x) * (_domeStation.x - playerPosition.x);
    float distanceY = (_domeStation.y - playerPosition.y) * (_domeStation.y - playerPosition.y);
    float distanceZ = (_domeStation.z - playerPosition.z) * (_domeStation.z - playerPosition.z);
    float distance = distanceX + distanceY + distanceZ;

    return distance + 0.01 <= _radiusSquared;
}

bool UpgradeManager::isTurretInRange()
{
    if(TURRETSMANAGER.RaycastTurrets() != -1)
    {
        return true;
    }
    return false;
}



