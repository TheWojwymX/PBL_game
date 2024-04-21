//
// Created by Jacek on 20.04.2024.
//

#define ENEMIESAIMANAGER EnemiesAIManager::getInstance()

#ifndef SANDBOX_ENEMIESAIMANAGER_H
#define SANDBOX_ENEMIESAIMANAGER_H

#include "Managers/ComponentsManager.h"
#include "EnemyAI.h"

#include "Managers/GameManager.h"

class EnemiesAIManager {

public:

    static EnemiesAIManager &getInstance();

    ~EnemiesAIManager() = default;

    EnemiesAIManager() = default;

    EnemiesAIManager(const EnemiesAIManager &) = delete;

    EnemiesAIManager &operator=(const EnemiesAIManager &) = delete;

    void Init();

    void Update();

    std::vector<std::shared_ptr<EnemyAI>> _enemiesAI;

};


#endif //SANDBOX_ENEMIESAIMANAGER_H
