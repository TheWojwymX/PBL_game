//
// Created by Jacek on 13.05.2024.
//

#ifndef SANDBOX_TURRETSMANAGER_H
#define SANDBOX_TURRETSMANAGER_H

#define TURRETSMANAGER TurretsManager::getInstance()

#include "Managers/ComponentsManager.h"
#include "Turret.h"
#include "Managers/NodesManager.h"
#include "Enemies/EnemiesManager.h"

class TurretsManager {
public:
    static TurretsManager &getInstance();

    ~TurretsManager() = default;

    TurretsManager() = default;

    TurretsManager(const TurretsManager &) = delete;

    TurretsManager &operator=(const TurretsManager &) = delete;

    void Init();

    void Update();

    std::vector<std::shared_ptr<Turret>> _turrets;

    void PrepareBlueprintTurret();

    void UpdateBlueprintTurret();

    bool _shouldEnableBlueprintTurret = false;

    void SpawnTurret();

    bool IsTooCloseToTurret(glm::vec3 pos);

    bool _isInBlueprintMode = false;

    float _distanceToAnotherTurret = 1.5f;

    float _sideRange = 5.0f;
    float _forwardRange = 7.0f;
    float _backRange = 2.0f;

    void CheckEnemiesInRange();

    bool isPointInRectangle(const glm::vec3 &M, const vector<glm::vec3> &rect);

    void AttackEnemy(const shared_ptr<Turret>& turret, const shared_ptr<Enemy>& enemy);

    void Reload(const shared_ptr<Turret> &turret);
};


#endif //SANDBOX_TURRETSMANAGER_H
