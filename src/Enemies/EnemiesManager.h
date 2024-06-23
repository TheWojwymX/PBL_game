#pragma once

#define ENEMIESMANAGER EnemiesManager::getInstance()

#include "Managers/ComponentsManager.h"
#include "Enemy.h"
#include "Core/Input.h"
#include "Managers/NodesManager.h"
#include "Managers/GameManager.h"

struct EnemyStats {
    float speed;
    float hp;
    float damage;
    float attackFrequency;
    float size;
    EnemyType type;

    // Constructor with default values
    EnemyStats(float spd = 1.0f, float health = 1, float dmg = 1, float freq = 1.0f, float sz = 1.0f, EnemyType tp = ANT)
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

    void SpawnEnemy(int distanceToAvoid, glm::vec3 scale, glm::vec2 spawnPos, EnemyType type);

    void ChooseModelBasedOnDistance();

    void ReturnToComingForNormalDestination(shared_ptr<Enemy> enemy);

    void CheckIfAtWalls(shared_ptr<Enemy> enemy);

    void AvoidEnemy(shared_ptr<Enemy> thisEnemy);
    void RemoveFromEnemies(shared_ptr<Enemy> enemy);
    void StartSpawning() { _finishedSpawning = false; }

    void Reset();

    void addAttackToGUI(shared_ptr<Enemy> enemy);
    void removeAttackFromGUI(std::shared_ptr<Enemy> enemy);
    glm::vec2 GetSpawnerPos(int i) { return _spawnersPositions[i]; }

    int _newEnemyIndex = 1;

    std::vector<std::shared_ptr<Node>> _enemiesParticles;
    std::vector<EnemyStats> _enemyStats;
    std::vector<glm::vec3> _roundsInfo;


private:
    int _spawnDistance;
    std::vector<glm::vec2> _spawnersPositions;
    glm::ivec3 _spawnedEnemies;
    float _spawnSpan;
    float _endlessTimer = 0.0f;
    float _endlessDelay;
    float _endlessDelayMin;
    float _endlessDelayStep;
    bool _finishedSpawning = false;

    void InitEnemyStats();
    glm::vec2 RandomSpawnPos();
    int CountValidEnemies();
};