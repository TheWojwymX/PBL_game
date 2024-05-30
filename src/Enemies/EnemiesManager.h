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

    glm::vec3 CalcClosestDomePosition(shared_ptr<Enemy> enemy);

    std::vector<glm::vec2> _spawnersPositions{
        glm::vec2(50.0, 90.0),
        glm::vec2(90.0, 50.0),
        glm::vec2(50.0, 2.0),
        glm::vec2(2.0, 50.0)
    };

    std::unordered_map<int, std::vector<std::pair<int, int>>> _roundsInfo;

    void SpawnEnemiesForRound(int roundNumber);

    glm::vec3 CalcRandomSpawnPosition(glm::vec2 spawnerPos);

    void SpawnEnemy(int distanceToAvoid, glm::vec3 scale, int spawnerIndex);

    void ChooseModelBasedOnDistance();

    void SetSymbolsForWave();

    void ReturnToComingForNormalDestination(shared_ptr<Enemy> enemy);

    void CheckIfAtWalls(shared_ptr<Enemy> enemy);

    void AvoidEnemy(shared_ptr<Enemy> thisEnemy);
};


#endif //SANDBOX_ENEMIESAIMANAGER_H
