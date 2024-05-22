//
// Created by Jacek on 20.04.2024.
//

#include "EnemiesManager.h"

EnemiesManager &EnemiesManager::getInstance() {
    static EnemiesManager instance;
    return instance;
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

    //Here you can modify rounds
    _roundsInfo[0] = {{0, 3}}; // Spawner 0 - spawn 3 enemies
    _roundsInfo[1] = {{1, 3}}; // Spawner 1 - spawn 3 enemies
    _roundsInfo[2] = {{1, 2}, {2, 2}}; //Spawner 1 - spawn 2 enemies, Spawner 2 - spawn 2 enemies
    _roundsInfo[3] = {{0, 3}, {2, 2}}; //Spawner 0 - spawn 3 enemies, Spawner 2 - spawn 2 enemies

}

void EnemiesManager::SpawnEnemiesForRound(int roundNumber) {
    if (_roundsInfo.find(roundNumber) != _roundsInfo.end()) {
        const auto &spawns = _roundsInfo[roundNumber];
        for (const auto &spawn : spawns) {
            for (int i = 0; i < spawn.second; ++i) {
                SpawnEnemy(2, glm::vec3(1.0f), spawn.first); // Example: spawn.first is spawnerIndex, spawn.second is enemyCount
            }
        }
    }
}



void EnemiesManager::ReturnToComingForNormalDestination(){
    for(int i = 0; i < _enemies.size(); i++){
        if(_enemies[i] == nullptr) continue;
        if(_enemies[i]->_destinationVector != CalcClosestDomePosition(_enemies[i]) && glm::distance(_enemies[i]->_destinationVector, _enemies[i]->GetOwnerPosition()) <= 1){
            _enemies[i]->_destinationVector = CalcClosestDomePosition(_enemies[i]);
            _enemies[i]->_isAvoiding = false;
        }
    }
}

void EnemiesManager::AvoidEnemy(){
    for(int i = 0; i < _enemies.size(); i++) {
        if(_enemies[i] == nullptr) continue;
/*        if(_enemies[i]->_isAvoiding){
            continue;
        }*/

        if(_enemies[i]->_isAtWalls){
            continue;
        }

        glm::vec3 avoidanceVector(0.0f);
        bool needsToAvoid = false;

        for(int j = 0; j < _enemies.size(); j++) {
            if(_enemies[j] == nullptr) continue;
            if (i == j) continue;

/*            if(_enemies[j]->_isAvoiding){
                continue;
            }*/

/*            if(_enemies[j]->_isAtWalls){
                continue;
            }*/

            glm::vec3 toOther = _enemies[j]->GetOwnerPosition() - _enemies[i]->GetOwnerPosition();
            float distanceToOther = glm::length(toOther);

            if (distanceToOther < (_enemies[j]->_size + _enemies[i]->_size)/2) {
                needsToAvoid = true;
                toOther = glm::normalize(toOther);
                avoidanceVector -= toOther / distanceToOther;
            }
        }

        if (needsToAvoid) {
            avoidanceVector = glm::normalize(avoidanceVector);
            _enemies[i]->_isAvoiding = true;
            avoidanceVector.y = 0;
            _enemies[i]->_destinationVector = _enemies[i]->GetOwnerPosition() + avoidanceVector * 3.0f;
            _enemies[i]->_distanceToStop = 1;
        } else {
            _enemies[i]->_isAvoiding = false;
            _enemies[i]->_distanceToStop = 0.1;
        }
    }
}

void EnemiesManager::Update() {

/*    for(int i = 0; i <= COMPONENTSMANAGER.Components.size() - 1; i++){
        std::cout << COMPONENTSMANAGER.Components[i]->_id << ", ";
    }*/

    ReturnToComingForNormalDestination();
    CheckIfAtWalls();
    AvoidEnemy();
    ChooseModelBasedOnDistance(); //LOD (safe to comment out)
    if(Input::Instance().IsKeyPressed(80)) {
        SpawnEnemy(2, glm::vec3(0.5, 0.5, 0.5));
    }
    if(Input::Instance().IsKeyPressed(79)){
        DealDamageToEnemy(50, _enemies[_testowaPrzeciwnicy]);
    }

    for(int i = 0; i < _enemies.size(); i++) {
        if(_enemies[i] == nullptr) continue;
        _enemies[i]->EnemyAI();
    }

}

void EnemiesManager::CheckIfAtWalls(){
    for(int i = 0; i < _enemies.size(); i++) {

        if(_enemies[i] == nullptr) continue;

        if(!_enemies[i]->_isAtWalls){

            //std::cout << "nie stoje pod murami i mam id " << _enemies[i]->_id << std::endl;

            if(glm::distance(_enemies[i]->GetOwnerPosition(), CalcClosestDomePosition(_enemies[i])) < 0.1f){
                _enemies[i]->_isAtWalls = true;
            }
        }
    }
}

void EnemiesManager::DealDamageToEnemy(int amount, const shared_ptr<Enemy>& enemy){

    if(enemy == nullptr) return;

    enemy->TakeDamage(amount);

}

void EnemiesManager::SpawnEnemy(int distanceToAvoid, glm::vec3 scale) {
    std::string nameOfEnemy = "Enemy" + to_string(_enemies.size() + 1);
    std::string particleGeneratorNode = "Particle" +  to_string(_enemies.size() + 1);

    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), nameOfEnemy);
    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), particleGeneratorNode);

    glm::vec3 enemyPosition = CalcRandomSpawnPosition(_spawnersPositions[0]);
    enemyPosition.y = 0.62 * scale.y + 99.49;
    NODESMANAGER.getNodeByName(nameOfEnemy)->GetTransform()->SetPosition(enemyPosition);
    NODESMANAGER.getNodeByName(nameOfEnemy)->GetTransform()->SetScale(scale);

    // Tutaj podepniemy potem animacje przy spawnie
    auto newMeshRenderer = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    newMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("antModel");
    newMeshRenderer->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
    newMeshRenderer->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    newMeshRenderer->Initiate();
    NODESMANAGER.getNodeByName(nameOfEnemy)->AddComponent(newMeshRenderer);

    auto newEnemyComponent = COMPONENTSMANAGER.CreateComponent<Enemy>();
    NODESMANAGER.getNodeByName(nameOfEnemy)->AddComponent(newEnemyComponent);
    newEnemyComponent->_destinationVector = CalcClosestDomePosition(newEnemyComponent);
    newEnemyComponent->_size = distanceToAvoid;
    _enemies.push_back(newEnemyComponent);

    auto antShot = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"),"antShot");
    antShot->SetOffset(glm::vec3(0.0f,0.0f,2.0f));
    antShot->object = NODESMANAGER.getNodeByName(nameOfEnemy);
    antShot->enemyScale = scale;
    antShot->_ownerNode = NODESMANAGER.getNodeByName(particleGeneratorNode);
    antShot->Init();
    NODESMANAGER.getNodeByName(particleGeneratorNode)->AddComponent(antShot);

    auto antDie = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"),"antDie");
    antDie->SetOffset(glm::vec3(0.0f,0.0f,2.0f));
    antDie->object = NODESMANAGER.getNodeByName(nameOfEnemy);
    antDie->enemyScale = scale;
    antDie->_ownerNode = NODESMANAGER.getNodeByName(particleGeneratorNode);
    antDie->Init();
    NODESMANAGER.getNodeByName(particleGeneratorNode)->AddComponent(antDie);

    //std::cout << newEnemyComponent->_destinationVector.x << "   " << newEnemyComponent->_destinationVector.z << std::endl;

    auto newAnimation = COMPONENTSMANAGER.CreateComponent<Animation>();

    //std::cout << NODESMANAGER.getNodeByName(nameOfEnemy)->_id << std::endl;
}

void EnemiesManager::SpawnEnemy(int distanceToAvoid, glm::vec3 scale, int spawnerIndex) {
    if (spawnerIndex >= 0 && spawnerIndex < _spawnersPositions.size()) {
        std::string nameOfEnemy = "Enemy" + std::to_string(_enemies.size() + 1);
        std::string particleGeneratorNode = "Particle" +  to_string(_enemies.size() + 1);

        NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), nameOfEnemy);
        NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), particleGeneratorNode);

        glm::vec3 enemyPosition = CalcRandomSpawnPosition(_spawnersPositions[spawnerIndex]);
        enemyPosition.y = 0.62 * scale.y + 99.49;
        NODESMANAGER.getNodeByName(nameOfEnemy)->GetTransform()->SetPosition(enemyPosition);
        NODESMANAGER.getNodeByName(nameOfEnemy)->GetTransform()->SetScale(scale);

        auto newMeshRenderer = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
        newMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("antModel");
        newMeshRenderer->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
        newMeshRenderer->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
        newMeshRenderer->Initiate();
        NODESMANAGER.getNodeByName(nameOfEnemy)->AddComponent(newMeshRenderer);

        auto antShot = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"),"antShot");
        antShot->SetOffset(glm::vec3(0.0f,0.0f,2.0f));
        antShot->object = NODESMANAGER.getNodeByName(nameOfEnemy);
        antShot->enemyScale = scale;
        antShot->_ownerNode = NODESMANAGER.getNodeByName(particleGeneratorNode);
        antShot->Init();
        NODESMANAGER.getNodeByName(particleGeneratorNode)->AddComponent(antShot);

        auto antDie = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"),"antDie");
        antDie->SetOffset(glm::vec3(0.0f,0.0f,2.0f));
        antDie->object = NODESMANAGER.getNodeByName(nameOfEnemy);
        antDie->enemyScale = scale;
        antDie->_ownerNode = NODESMANAGER.getNodeByName(particleGeneratorNode);
        antDie->Init();
        NODESMANAGER.getNodeByName(particleGeneratorNode)->AddComponent(antDie);


        auto newEnemyComponent = COMPONENTSMANAGER.CreateComponent<Enemy>();
        NODESMANAGER.getNodeByName(nameOfEnemy)->AddComponent(newEnemyComponent);
        newEnemyComponent->_destinationVector = CalcClosestDomePosition(newEnemyComponent);
        newEnemyComponent->_size = distanceToAvoid;
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
    vector = glm::vec2(GAMEMANAGER._domePosition.x + GAMEMANAGER._domeRadius * vector.x, GAMEMANAGER._domePosition.y + GAMEMANAGER._domeRadius * vector.y);

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