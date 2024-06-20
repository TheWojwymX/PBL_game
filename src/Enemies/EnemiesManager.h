#pragma once

#define ENEMIESMANAGER EnemiesManager::getInstance()

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

    void SpawnEnemiesForRound();

    glm::vec3 CalcRandomSpawnPosition(glm::vec2 spawnerPos);

    void SpawnEnemy(int distanceToAvoid, glm::vec3 scale, int spawnerIndex, EnemyType type, float speedScale = 1.0f);

    void ChooseModelBasedOnDistance();

    void ReturnToComingForNormalDestination(shared_ptr<Enemy> enemy);

    void CheckIfAtWalls(shared_ptr<Enemy> enemy);

    void AvoidEnemy(shared_ptr<Enemy> thisEnemy);

    void Reset();

    void addAttackToGUI(shared_ptr<Enemy> enemy);
    void removeAttackFromGUI(std::shared_ptr<Enemy> enemy);

    int _newEnemyIndex = 1;

    std::vector<std::shared_ptr<Node>> _enemiesParticles;

private:
    std::vector<EnemyStats> _enemyStats;
    int _spawnerDistance;
    std::vector<glm::vec2> _spawnersPositions;
    std::vector<glm::vec3> _roundsInfo;
    glm::ivec3 _spawnedEnemies;
    float _spawnSpan;

    void InitEnemyStats();
};