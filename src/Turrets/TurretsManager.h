//
// Created by Jacek on 13.05.2024.
//

#ifndef SANDBOX_TURRETSMANAGER_H
#define SANDBOX_TURRETSMANAGER_H

#include "Managers/ComponentsManager.h"
#include "Turret.h"
#include "Managers/NodesManager.h"

class TurretsManager {
    static TurretsManager &getInstance();

    ~TurretsManager() = default;

    TurretsManager() = default;

    TurretsManager(const TurretsManager &) = delete;

    TurretsManager &operator=(const TurretsManager &) = delete;

    void Init();

    void Update();

    std::vector<std::shared_ptr<Turret>> _turrets;
};


#endif //SANDBOX_TURRETSMANAGER_H
