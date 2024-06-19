#define ENEMIESMANAGER EnemiesManager::getInstance()

#pragma once

#include "Managers/ComponentsManager.h"
#include "Enemy.h"
#include "Core/Input.h"
#include "Managers/NodesManager.h"

#include "Managers/GameManager.h"

struct EnemyStats {
    float speed;
    int hp;
    int damage;
    float attackFrequency;
    float size;
    EnemyType type;

    // Constructor with default values
    EnemyStats(float spd = 1.0f, int health = 1, int dmg = 1, float freq = 1.0f, float sz = 1.0f, EnemyType tp = ANT)
        : speed(spd), hp(health), damage(dmg), attackFrequency(freq), size(sz), type(tp) {}
};

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
        glm::vec2(50.0, 150.0),
        glm::vec2(150.0, 50.0),
        glm::vec2(110.0, 2.0),
        glm::vec2(2.0, 110.0)
    };

    std::unordered_map<int, std::vector<std::tuple<int, int, EnemyType>>> _roundsInfo;

    void SpawnEnemiesForRound(int roundNumber);

    glm::vec3 CalcRandomSpawnPosition(glm::vec2 spawnerPos);

    void SpawnEnemy(int distanceToAvoid, glm::vec3 scale, int spawnerIndex, EnemyType type);

    void ChooseModelBasedOnDistance();

    void ReturnToComingForNormalDestination(shared_ptr<Enemy> enemy);

    void CheckIfAtWalls(shared_ptr<Enemy> enemy);

    void AvoidEnemy(shared_ptr<Enemy> thisEnemy);

    void Reset();

    int _newEnemyIndex = 1;

    std::vector<std::shared_ptr<Node>> _enemiesParticles;

private:
    std::vector<EnemyStats> _enemyStats;

    void InitEnemyStats();
};