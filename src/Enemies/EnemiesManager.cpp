//
// Created by Jacek on 20.04.2024.
//

#include "EnemiesManager.h"

EnemiesManager &EnemiesManager::getInstance() {
    static EnemiesManager instance;
    return instance;
}

void EnemiesManager::Update() {
    ChooseModelBasedOnDistance(); //LOD (safe to comment out)
    if(Input::Instance().IsKeyPressed(GLFW_KEY_P)) {

        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.5f, 1.0f);
        float scale = dis(gen);

        SpawnEnemy(2.5, glm::vec3(scale), 0, ANT);
    }

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

    _roundsInfo[0] = {{0, 3, ANT}, {0, 3, BEETLE}, {0, 3, WASP}}; // TEST

    _roundsInfo[1] = {{2, 3, ANT}}; // Spawner 1 - spawn 3 ANT enemies
    _roundsInfo[2] = {{2, 2, ANT}, {1, 2, ANT}}; // Spawner 2 - spawn 2 ANT enemies, Spawner 1 - spawn 2 ANT enemies
    _roundsInfo[3] = {{3, 3, ANT}, {2, 2, ANT}}; // Spawner 3 - spawn 3 ANT enemies, Spawner 2 - spawn 2 ANT enemies

    // TEMPORARY
    _roundsInfo[4] = {{0, 3, ANT}}; // Spawner 0 - spawn 3 ANT enemies
    _roundsInfo[5] = {{1, 3, ANT}}; // Spawner 1 - spawn 3 ANT enemies
    _roundsInfo[6] = {{2, 2, ANT}, {1, 2, ANT}}; // Spawner 2 - spawn 2 ANT enemies, Spawner 1 - spawn 2 ANT enemies
    _roundsInfo[7] = {{3, 3, ANT}, {2, 2, ANT}}; // Spawner 3 - spawn 3 ANT enemies, Spawner 2 - spawn 2 ANT enemies

}

void EnemiesManager::SpawnEnemiesForRound(int roundNumber)
{
    if (_roundsInfo.find(roundNumber) != _roundsInfo.end()) {
        const auto &spawns = _roundsInfo[roundNumber];
        for (const auto &spawn : spawns) {
            int spawnerIndex = std::get<0>(spawn);
            int enemyCount = std::get<1>(spawn);
            enemyType type = std::get<2>(spawn);

            for (int i = 0; i < enemyCount; ++i) {
                std::random_device rd;
                std::mt19937 gen(rd());
                std::uniform_real_distribution<float> dis(0.5f, 1.0f);
                float scale = dis(gen);

                SpawnEnemy(2.5, glm::vec3(scale), spawnerIndex, type);
            }
        }
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

void EnemiesManager::CheckIfAtWalls(shared_ptr<Enemy> enemy)
{
    if(enemy == nullptr) return;

    if(!enemy->_isAtWalls){
        if(glm::distance(enemy->GetOwnerPosition(), CalcClosestDomePosition(enemy)) < 0.1f){
            enemy->_isAtWalls = true;
            auto animation = enemy->GetOwnerNode()->GetComponent<Animation>();
            if(animation != nullptr && animation->_enemyState != DEAD)
            {
                enemy->GetOwnerNode()->GetComponent<Animation>()->_enemyState=state::ATTACK;
            }
        }
    }
}

void EnemiesManager::SpawnEnemy(int distanceToAvoid, glm::vec3 scale, int spawnerIndex, enemyType type)
{

//    std::cout << "Ilosc mrowek to: " << _enemies.size() << std::endl;

    if (spawnerIndex >= 0 && spawnerIndex < _spawnersPositions.size())
    {
        std::string nameOfEnemy = "Enemy" + std::to_string(_enemies.size() + 1);
        std::string particleGeneratorNode = "Particle" +  to_string(_enemies.size() + 1);

        NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), nameOfEnemy);
        NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), particleGeneratorNode);

        glm::vec3 enemyPosition = CalcRandomSpawnPosition(_spawnersPositions[spawnerIndex]);
        enemyPosition.y = 0.62 * scale.y + 299.49;

        if (type == WASP)
        {
            enemyPosition.y = enemyPosition.y + 1.5;
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
        newAnimation->InitComponent((int)(type + 1));
        NODESMANAGER.getNodeByName(nameOfEnemy)->AddComponent(newAnimation);

        auto antShot = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"),"antShot");
        antShot->SetOffset(glm::vec3(0.0f,0.0f,1.0f));
        antShot->object = NODESMANAGER.getNodeByName(nameOfEnemy);
        antShot->enemyScale = scale;
        antShot->_ownerNode = NODESMANAGER.getNodeByName(particleGeneratorNode);
        antShot->Init();
        NODESMANAGER.getNodeByName(particleGeneratorNode)->AddComponent(antShot);

        auto antDie = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"),"antDie");
        antDie->SetOffset(glm::vec3(0.0f,0.0f,1.0f));
        antDie->object = NODESMANAGER.getNodeByName(nameOfEnemy);
        antDie->enemyScale = scale;
        antDie->_ownerNode = NODESMANAGER.getNodeByName(particleGeneratorNode);
        antDie->Init();
        NODESMANAGER.getNodeByName(particleGeneratorNode)->AddComponent(antDie);

        auto newEnemyComponent = COMPONENTSMANAGER.CreateComponent<Enemy>();
        NODESMANAGER.getNodeByName(nameOfEnemy)->AddComponent(newEnemyComponent);
        newEnemyComponent->_destinationVector = CalcClosestDomePosition(newEnemyComponent);
        newEnemyComponent->_size = distanceToAvoid;
        newEnemyComponent->_enemyType = type;
        newEnemyComponent->setUp();

        _enemies.push_back(newEnemyComponent);
    }
}


glm::vec3 EnemiesManager::CalcRandomSpawnPosition(glm::vec2 spawnerPos){
    float spawnerRadius = 15.0f;
    float angle = static_cast<float>(std::rand()) / RAND_MAX * 2.0f * static_cast<float>(3.141592);
    float randomRadius = static_cast<float>(std::rand()) / RAND_MAX;
    randomRadius = std::sqrt(randomRadius);
    randomRadius *= spawnerRadius;
    float x = spawnerPos.x + randomRadius * std::cos(angle);
    float z = spawnerPos.y + randomRadius * std::sin(angle);
    return glm::vec3(x, 99.8f, z);
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
