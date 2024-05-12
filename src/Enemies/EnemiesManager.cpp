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
    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), nameOfEnemy);

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

    //std::cout << newEnemyComponent->_destinationVector.x << "   " << newEnemyComponent->_destinationVector.z << std::endl;

    auto newAnimation = COMPONENTSMANAGER.CreateComponent<Animation>();

    //std::cout << NODESMANAGER.getNodeByName(nameOfEnemy)->_id << std::endl;
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
