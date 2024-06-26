#include "EnemiesManager.h"


EnemiesManager &EnemiesManager::getInstance() {
    static EnemiesManager instance;
    return instance;
}

EnemiesManager::EnemiesManager(): _scaleRand(30,0.7f,1.3f), _endlessRand(100,0.0f,1.0f), _gen(_rd())
{
}

void EnemiesManager::Update() {
    //ChooseModelBasedOnDistance(); //LOD (safe to comment out)

    SpawnEnemiesForRound();

    for(int i = 0; i < _enemies.size(); i++)
    {
        if(_enemies[i] != nullptr)
        {
            _enemies[i]->EnemyAI();
            ReturnToComingForNormalDestination(_enemies[i]);
            CheckIfAtWalls(_enemies[i]);
            AvoidEnemy(_enemies[i]);
        }
    }

}

void EnemiesManager::Init() {
    for(int i = 0; i < COMPONENTSMANAGER.GetComponents().size(); i++){
        if(COMPONENTSMANAGER.GetComponents()[i] != nullptr && COMPONENTSMANAGER.GetComponents()[i]->_type == ComponentType::ENEMYAI){
            _enemies.push_back(std::dynamic_pointer_cast<Enemy>(COMPONENTSMANAGER.GetComponents()[i]));
        }
    }

    for(int i = 0; i < _enemies.size(); i++){
        _enemies[i]->_destinationVector = CalcClosestDomePosition(_enemies[i]);
    }

    _spawnersPositions = {
    glm::vec2(GAMEMANAGER._domePosition.x,GAMEMANAGER._domePosition.y + 90),
    glm::vec2(GAMEMANAGER._domePosition.x,GAMEMANAGER._domePosition.y - 80)
    };

    //ant , beetle, wasp
    _roundsInfo = {
        glm::vec3(10, 0, 0), // 0
        glm::vec3(25, 1, 0), // 1
        glm::vec3(50, 3, 1), // 2
        glm::vec3(80, 6, 3), // 3
        glm::vec3(115, 10, 6), // 4
        glm::vec3(155, 15, 10), // 5
        glm::vec3(200, 21, 15), // 6
        glm::vec3(250, 28, 21), // 7
        glm::vec3(305, 36, 28), // 8
        glm::vec3(365, 45, 36), // 9
        glm::vec3(0.85f, 0.05f, 0.1f) // endless
    };

    _spawnedEnemies = glm::ivec3(0);
    _spawnDistance = 100;
    _endlessDelay = 3.0f;
    _endlessDelayMin = 0.1f;
    _endlessDelayStep = 0.025f;
    _spawnSpan = 0.5f;

    InitEnemyStats();
    InitAnimationFrames();
}

void EnemiesManager::SpawnEnemiesForRound()
{
    if (GAMEMANAGER.GetPhase() != Phase::DEFEND || _finishedSpawning) return;

    if (!GAMEMANAGER.IsEndless()) {
        float spawnPortion = GAMEMANAGER.GetPhaseProgress() / _spawnSpan;
        spawnPortion = glm::clamp(spawnPortion, 0.0f, 1.0f);

        glm::ivec3 spawns = _roundsInfo[GAMEMANAGER.GetRoundNumber()] * spawnPortion;

        if (spawns == _spawnedEnemies) return;

        glm::ivec3 diff = spawns - _spawnedEnemies;
        _spawnedEnemies = spawns;

        for (int i = 0; i < 3; i++) {
            for (int j = 0; j < diff[i]; j++) {
                float scale = _scaleRand.GetRandomFloat();
                SpawnEnemy(_enemyStats[i].size * scale, glm::vec3(scale), RandomSpawnPos(), static_cast<EnemyType>(i));
            }
        }
        if (spawnPortion == 1.0f) _finishedSpawning = true;
    }
    else {
        _endlessTimer += TIME.GetDeltaTime();
        if (_endlessDelay > _endlessDelayMin) _endlessDelay -= _endlessDelayStep * TIME.GetDeltaTime();
        if (_endlessTimer < _endlessDelay) return;

        _endlessTimer = 0.0f; 

        const glm::vec3& weights = _roundsInfo[GAMEMANAGER.GetRoundNumber()];

        float randomValue = _endlessRand.GetRandomFloat();

        int chosenEnemyType = -1;
        if (randomValue < weights.x) {
            chosenEnemyType = 0;
        }
        else if (randomValue < weights.x + weights.y) {
            chosenEnemyType = 1;
        }
        else {
            chosenEnemyType = 2;
        }

        float scale = _scaleRand.GetRandomFloat();
        SpawnEnemy(_enemyStats[chosenEnemyType].size * scale, glm::vec3(scale), RandomSpawnPos(), static_cast<EnemyType>(chosenEnemyType));
    }
}

void EnemiesManager::ReturnToComingForNormalDestination(shared_ptr<Enemy> enemy)
{
    if(enemy == nullptr) return;

    if(enemy->_destinationVector != CalcClosestDomePosition(enemy) && glm::distance(enemy->_destinationVector, enemy->GetOwnerPosition()) <= 1){
        enemy->_destinationVector = CalcClosestDomePosition(enemy);
        enemy->_isAvoiding = false;
    }
}

void EnemiesManager::AvoidEnemy(shared_ptr<Enemy> thisEnemy)
{
    if (thisEnemy == nullptr) return;

    if (thisEnemy->_isAtWalls) {
        return;
    }

    glm::vec3 avoidanceVector(0.0f);
    bool needsToAvoid = false;

    for (int j = 0; j < _enemies.size(); j++) {
        if (_enemies[j] == nullptr) continue;
        if (thisEnemy == _enemies[j]) continue;

        bool shouldAvoid = false;

        // Ants and beetles avoid each other
        if ((thisEnemy->_enemyType == ANT || thisEnemy->_enemyType == BEETLE) &&
            (_enemies[j]->_enemyType == ANT || _enemies[j]->_enemyType == BEETLE)) {
            shouldAvoid = true;
        }

        // Wasps avoid other wasps
        if (thisEnemy->_enemyType == WASP && _enemies[j]->_enemyType == WASP) {
            shouldAvoid = true;
        }

        if (shouldAvoid) {
            glm::vec3 toOther = _enemies[j]->GetOwnerPosition() - thisEnemy->GetOwnerPosition();
            float distanceToOther = glm::length(toOther);

            if (distanceToOther < (_enemies[j]->_size + thisEnemy->_size) / 2) {
                needsToAvoid = true;
                toOther = glm::normalize(toOther);
                avoidanceVector -= toOther / distanceToOther;
            }
        }
    }

    if (needsToAvoid) {
        avoidanceVector = glm::normalize(avoidanceVector);
        thisEnemy->_isAvoiding = true;
        avoidanceVector.y = 0;
        thisEnemy->_destinationVector = thisEnemy->GetOwnerPosition() + avoidanceVector * 3.0f;
        thisEnemy->_distanceToStop = 1;
    } else {
        thisEnemy->_isAvoiding = false;
        thisEnemy->_distanceToStop = 0.1;
    }
}

void EnemiesManager::RemoveFromEnemies(shared_ptr<Enemy> enemy)
{
    auto it = std::find(ENEMIESMANAGER._enemies.begin(), ENEMIESMANAGER._enemies.end(), enemy);
    if (it != ENEMIESMANAGER._enemies.end()) {
        *it = nullptr;
    }

    if (CountValidEnemies() == 0 && _finishedSpawning) GAMEMANAGER.RoundWon();
}

void EnemiesManager::CheckIfAtWalls(shared_ptr<Enemy> enemy)
{
    if(enemy == nullptr) return;

    if(!enemy->_isAtWalls){
        if(glm::distance(enemy->GetOwnerPosition(), CalcClosestDomePosition(enemy)) < 0.5f){
            enemy->_isAtWalls = true;
            auto animation = enemy->GetOwnerNode()->GetComponent<Animation>();
            if(animation != nullptr && animation->_enemyState != DEAD)
            {
                enemy->GetOwnerNode()->GetComponent<Animation>()->_enemyState=state::ATTACK;
            }
            addAttackToGUI(enemy);
        }
    }
}

void EnemiesManager::SpawnEnemy(int distanceToAvoid, glm::vec3 scale, glm::vec2 spawnPos, EnemyType type)
{
    std::string nameOfEnemy = "Enemy" + std::to_string(_newEnemyIndex);
    std::string particleGeneratorNode = "Particle" +  to_string(_newEnemyIndex);

    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), nameOfEnemy);
    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), particleGeneratorNode);

    glm::vec3 enemyPosition = glm::vec3(spawnPos.x, GAMEMANAGER._groundLevel, spawnPos.y);

    if (type == WASP)
    {
        enemyPosition.y = enemyPosition.y + 5;
    }

    NODESMANAGER.getNodeByName(nameOfEnemy)->GetTransform()->SetPosition(enemyPosition);
    NODESMANAGER.getNodeByName(nameOfEnemy)->GetTransform()->SetScale(scale);

    auto newMeshRenderer = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    newMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("antModel");
    newMeshRenderer->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
    newMeshRenderer->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    newMeshRenderer->Initiate();
    NODESMANAGER.getNodeByName(nameOfEnemy)->AddComponent(newMeshRenderer);

    auto newAnimation = COMPONENTSMANAGER.CreateComponent<Animation>();
    newAnimation->setMeshRenderer(NODESMANAGER.getNodeByName(nameOfEnemy)->GetComponent<MeshRenderer>());
    newAnimation->setMeshRendererId(COMPONENTSMANAGER._nextComponentID-1);
    newAnimation->InitComponent(type,_animationFrames[type].GetAllFrames());
    NODESMANAGER.getNodeByName(nameOfEnemy)->AddComponent(newAnimation);

    auto antSpawn = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"),"antSpawn");
    antSpawn->SetOffset(glm::vec3(0.0f,0.0f,1.0f));
    antSpawn->object = NODESMANAGER.getNodeByName(nameOfEnemy);
    antSpawn->enemyScale = scale;
    antSpawn->_ownerNode = NODESMANAGER.getNodeByName(particleGeneratorNode);
    antSpawn->Init();
    NODESMANAGER.getNodeByName(particleGeneratorNode)->AddComponent(antSpawn);
    antSpawn->SpawnParticles();

    auto antShot = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"),"antShot");
    antShot->SetOffset(glm::vec3(0.0f,0.0f,1.0f));
    antShot->object = NODESMANAGER.getNodeByName(nameOfEnemy);
    antShot->enemyScale = scale;
    antShot->_ownerNode = NODESMANAGER.getNodeByName(particleGeneratorNode);
    antShot->Init();
    NODESMANAGER.getNodeByName(particleGeneratorNode)->AddComponent(antShot);

    string particleType;
    if(type == WASP) particleType = "waspDie";
    if(type == ANT) particleType = "antDie";
    if(type == BEETLE) particleType = "beetleDie";

    auto antDie = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"),particleType);
    antDie->SetOffset(glm::vec3(0.0f,0.0f,1.0f));
    antDie->object = NODESMANAGER.getNodeByName(nameOfEnemy);
    antDie->enemyScale = scale;
    antDie->_ownerNode = NODESMANAGER.getNodeByName(particleGeneratorNode);
    antDie->Init();
    NODESMANAGER.getNodeByName(particleGeneratorNode)->AddComponent(antDie);

    auto newEnemyComponent = COMPONENTSMANAGER.CreateComponent<Enemy>();
    NODESMANAGER.getNodeByName(nameOfEnemy)->AddComponent(newEnemyComponent);
    newEnemyComponent->_destinationVector = CalcClosestDomePosition(newEnemyComponent);

    glm::vec3 targetLookDirection = newEnemyComponent->_destinationVector - newEnemyComponent->GetOwnerNode()->GetTransform()->GetPosition();
    targetLookDirection.y = 0.0f;

    if (glm::length(targetLookDirection) > 0.0f) {
        targetLookDirection = glm::normalize(targetLookDirection);

        // Calculate the yaw angle (in radians)
        float targetYaw = atan2(targetLookDirection.x, targetLookDirection.z);
        glm::quat targetRotation = glm::angleAxis(targetYaw, glm::vec3(0, 1, 0));
        newEnemyComponent->_ownerNode->GetTransform()->SetRotation(targetRotation);
    }


    EnemyStats stats = _enemyStats[type];
    newEnemyComponent->SetStats(stats.type, stats.speed, stats.hp,stats.damage,stats.attackFrequency,stats.size * scale.x);

    _enemies.push_back(newEnemyComponent);
    _enemiesParticles.push_back(NODESMANAGER.getNodeByName(particleGeneratorNode));
    _newEnemyIndex++;
}

glm::vec3 EnemiesManager::CalcClosestDomePosition(shared_ptr<Enemy> enemy){
    glm::vec2 vector = glm::vec2(enemy->GetOwnerPosition().x - GAMEMANAGER._domePosition.x, enemy->GetOwnerPosition().z - GAMEMANAGER._domePosition.y);
    vector = glm::normalize(vector);
    vector = glm::vec2(GAMEMANAGER._domePosition.x + (GAMEMANAGER._domeRadius + 4.5) * vector.x, GAMEMANAGER._domePosition.y + (GAMEMANAGER._domeRadius + 4.5) * vector.y);

    return glm::vec3(vector.x, enemy->GetOwnerPosition().y, vector.y);
}

void EnemiesManager::ChooseModelBasedOnDistance() {
    glm::vec3 playerCamera = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition();
    shared_ptr<Model> normalModel = RESOURCEMANAGER.GetModelByName("antModel");
    shared_ptr<Model> farModel = RESOURCEMANAGER.GetModelByName("sandModel");     //Change Model
    float lodDistance = 40.0f;                                                          //Change Distance

    for(int i = 0; i < _enemies.size(); i++){
        if(_enemies[i] == nullptr) continue;
        float distance = glm::length(_enemies[i]->GetOwnerPosition() - playerCamera);
        if(distance > lodDistance){
            if(_enemies[i]->_ownerNode->GetComponent<MeshRenderer>()->_model != farModel){
                _enemies[i]->_ownerNode->GetComponent<MeshRenderer>()->_model = farModel;
            }
        }
        else{
            if(_enemies[i]->_ownerNode->GetComponent<MeshRenderer>()->_model != normalModel){
                _enemies[i]->_ownerNode->GetComponent<MeshRenderer>()->_model = normalModel;
            }
        }
    }
}

void EnemiesManager::Reset() {
    for (int i = 0; i < _enemies.size(); i++) {
        if (_enemies[i] == nullptr) continue;
        NODESMANAGER.getNodeByName("root")->RemoveChild(_enemies[i]->_ownerNode);
    }
    _enemies.clear();

    for (int i = 0; i < _enemiesParticles.size(); i++) {
        if (_enemiesParticles[i] == nullptr) continue;
        NODESMANAGER.getNodeByName("root")->RemoveChild(_enemiesParticles[i]);
    }
    _enemiesParticles.clear();
    _finishedSpawning = false;

    resetAttackAtGUI();
}

void EnemiesManager::InitEnemyStats() {
    // Clear the vector if it already contains elements (optional)
    _enemyStats.clear();

    // speed, hp, damage, attackFrequency, size, type
    EnemyStats ant(7.0f, 3, 0.5f, 0.6f, 2.0f, ANT);
    EnemyStats beetle(3.0f, 30, 5, 1.5f, 4.0f, BEETLE);
    EnemyStats wasp(10.0f, 3, 1, 0.45f, 2.0f, WASP);

    // Add all objects to the _enemyStats vector
    _enemyStats.push_back(ant);
    _enemyStats.push_back(beetle);
    _enemyStats.push_back(wasp);
}

void EnemiesManager::InitAnimationFrames()
{
    // Initialize Ant Animation Frames
    AnimationFrames antAnimationFrames;
    antAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("AntSpawn0"));
    antAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("AntSpawn1"));
    antAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("AntSpawn2"));
    antAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("AntSpawn3"));
    antAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("AntSpawn4"));

    antAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("AntWalk0"));
    antAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("AntWalk1"));
    antAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("AntWalk2"));
    antAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("AntWalk3"));
    antAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("AntWalk4"));
    antAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("AntWalk5"));

    antAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("AntAttack0"));
    antAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("AntAttack1"));
    antAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("AntAttack2"));
    antAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("AntAttack3"));

    antAnimationFrames.deadFrames.push_back(RESOURCEMANAGER.GetModelByName("AntDead0"));
    antAnimationFrames.deadFrames.push_back(RESOURCEMANAGER.GetModelByName("AntDead1"));
    antAnimationFrames.deadFrames.push_back(RESOURCEMANAGER.GetModelByName("AntDead2"));

    _animationFrames.push_back(std::move(antAnimationFrames));

    // Initialize Beetle Animation Frames
    AnimationFrames beetleAnimationFrames;
    beetleAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleSpawn0"));
    beetleAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleSpawn1"));
    beetleAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleSpawn2"));
    beetleAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleSpawn3"));
    beetleAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleSpawn4"));
    beetleAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleSpawn5"));

    beetleAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleWalk0"));
    beetleAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleWalk1"));
    beetleAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleWalk2"));
    beetleAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleWalk3"));
    beetleAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleWalk4"));
    beetleAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleWalk5"));

    beetleAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleAttack0"));
    beetleAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleAttack1"));
    beetleAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleAttack2"));
    beetleAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleAttack3"));
    beetleAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleAttack4"));
    beetleAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleAttack5"));
    beetleAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleAttack6"));
    beetleAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleAttack7"));
    beetleAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleAttack8"));
    beetleAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleAttack9"));

    beetleAnimationFrames.deadFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleDead0"));
    beetleAnimationFrames.deadFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleDead1"));
    beetleAnimationFrames.deadFrames.push_back(RESOURCEMANAGER.GetModelByName("BeetleDead2"));

    _animationFrames.push_back(std::move(beetleAnimationFrames));

    // Initialize Wasp Animation Frames
    AnimationFrames waspAnimationFrames;
    waspAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspSpawn0"));
    waspAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspSpawn1"));
    waspAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspSpawn2"));
    waspAnimationFrames.spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspSpawn3"));

    waspAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspFly0"));
    waspAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspFly1"));
    waspAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspFly2"));
    waspAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspFly3"));
    waspAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspFly4"));
    waspAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspFly5"));
    waspAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspFly6"));
    waspAnimationFrames.walkFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspFly7"));

    waspAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspAttack0"));
    waspAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspAttack1"));
    waspAnimationFrames.attackFrames.push_back(RESOURCEMANAGER.GetModelByName("WaspAttack2"));

    waspAnimationFrames.deadFrames.push_back(nullptr);
    // Add waspAnimationFrames to the _animationFrames vector
    _animationFrames.push_back(std::move(waspAnimationFrames));
}

glm::vec2 EnemiesManager::RandomSpawnPos() {
    // Get the current round number
    int currentRound = GAMEMANAGER.GetRoundNumber();

    // Calculate the maximum angle based on the current round
    float maxAngle = std::min(360.0f, 90.0f * (currentRound + 1));

    // Create the random distribution between 0 and maxAngle
    std::uniform_real_distribution<float> angleDis(0.0f, glm::radians(maxAngle));

    // Generate a random angle
    float angle = angleDis(_gen);

    // Apply _roundRandom to offset the angle
    float adjustedAngle = angle + _roundRandom * glm::radians(maxAngle);

    // Ensure the angle wraps around correctly
    adjustedAngle = std::fmod(adjustedAngle, 2.0f * glm::pi<float>());

    // Calculate x and y based on the adjusted angle
    float x = _spawnDistance * std::cos(adjustedAngle);
    float y = _spawnDistance * std::sin(adjustedAngle);

    // Get the center position of the circle (dome position)
    glm::vec2 domePos = glm::vec2(GAMEMANAGER._domePosition);

    // Return the position relative to the center
    return domePos + glm::vec2(x, y);
}

int EnemiesManager::CountValidEnemies()
{
    int count = 0;
    for (const auto& enemy : _enemies) {
        if (enemy != nullptr) {
            count++;
        }
    }
    return count;
}


void EnemiesManager::addAttackToGUI(shared_ptr<Enemy> enemy)
{
    shared_ptr<ImageRenderer> newArrow = make_shared<ImageRenderer>();
    newArrow->Init("res/Images/enemy_arrow.png", CoordsConverter::ConvertCoords(glm::vec2(1789, 139)), 90, true, true);
    HUD._attackSymbols.push_back(std::make_pair(newArrow, enemy));
}

void EnemiesManager::removeAttackFromGUI(std::shared_ptr<Enemy> enemy)
{
    auto it = std::find_if(HUD._attackSymbols.begin(), HUD._attackSymbols.end(),
                           [&enemy](const std::pair<std::shared_ptr<ImageRenderer>, std::shared_ptr<Enemy>>& pair)
                           {
                               return pair.second == enemy;
                           });

    if (it != HUD._attackSymbols.end())
    {
        HUD._attackSymbols.erase(it);
    }
}

void EnemiesManager::resetAttackAtGUI()
{
    for (int i = 0; i < HUD._attackSymbols.size(); i++)
    {
        HUD._attackSymbols[i].first == nullptr;
        HUD._attackSymbols[i].second == nullptr;
    }
    HUD._attackSymbols.clear();
}
