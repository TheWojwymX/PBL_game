//
// Created by Jacek on 20.04.2024.
//

#define ENEMIESMANAGER EnemiesManager::getInstance()

#ifndef SANDBOX_ENEMIESAIMANAGER_H
#define SANDBOX_ENEMIESAIMANAGER_H

#include "Managers/ComponentsManager.h"
#include "Enemy.h"
#include "Core/Input.h"
#include "Managers/NodesManager.h"

#include "Managers/GameManager.h"

class EnemiesManager {

public:

    static EnemiesManager &getInstance();

    ~EnemiesManager() = default;

    EnemiesManager() = default;

    EnemiesManager(const EnemiesManager &) = delete;

    EnemiesManager &operator=(const EnemiesManager &) = delete;

    void Init();

    void Update();

    std::vector<std::shared_ptr<Enemy>> _enemies;

    int _enemiesSize = 4;

    glm::vec3 destinationVectorWalking;
    glm::vec3 destinationVectorFlying;

    float _distanceToStop;

    void AvoidEnemy();

    void ReturnToComingForNormalDestination();

    glm::vec3 CalcClosestDomePosition(shared_ptr<Enemy> enemy);

    std::vector<glm::vec2> _spawnersPositions{
        glm::vec2(62.0, 100.0),
        glm::vec2(-62.0, 100.0),
    };

    glm::vec3 CalcRandomSpawnPosition(glm::vec2 spawnerPos);

    void CheckIfAtWalls();

    void DealDamageToEnemy(int amount, const shared_ptr<Enemy> &enemy);

    int _testowaPrzeciwnicy = 3;

    void SpawnEnemy(int distanceToAvoid, glm::vec3 scale);

    void ChooseModelBasedOnDistance();
};


#endif //SANDBOX_ENEMIESAIMANAGER_H
