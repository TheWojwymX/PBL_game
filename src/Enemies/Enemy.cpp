//
// Created by Jacek on 20.04.2024.
//

#include "Enemy.h"
#include "Core/Time.h"
#include "EnemiesManager.h"
#include "Managers/DomeManager.h"

Enemy::Enemy() {
    _type = ComponentType::ENEMYAI;
}

nlohmann::json Enemy::Serialize() {
    nlohmann::json data;

    data["enemySize"] = _size;

    return data;
}

void Enemy::Deserialize(const nlohmann::json &jsonData) {

    if (jsonData.contains("enemySize")) {
        _size = jsonData["enemySize"].get<float>();
    }

    Component::Deserialize(jsonData);
}

void Enemy::WalkToDestination(glm::vec3 *destination) {

    if(_isAtWalls) return;

    if(destination != nullptr){
        _destinationVector = *destination;
    }

    if(glm::distance(_destinationVector, _ownerTransform->GetPosition()) > _distanceToStop && !_shouldStay){

        _slalomTime += TIME.GetDeltaTime();
        float sideMovement = sin(_slalomTime * _slalomFrequency) * _slalomAmplitude;
        glm::vec3 sideVector = glm::normalize(glm::cross(_ownerTransform->GetRotation() * glm::vec3(0, 1, 0), _ownerTransform->GetRotation() * glm::vec3(0, 0, -1))) * sideMovement;

        glm::vec3 currentPos = _ownerTransform->GetPosition();
        glm::vec3 forwardMovement = Transform::MoveTowards(currentPos, _destinationVector, 0.01f * _walkingSpeed) - currentPos;

        glm::vec3 effectiveMovement = forwardMovement + sideVector;
        glm::vec3 movementDirection = glm::normalize(effectiveMovement);
        movementDirection.y = 0.0f;

        _ownerTransform->LookAt(movementDirection);
        _ownerTransform->SetPosition(currentPos + effectiveMovement);

    }
}

void Enemy::Die(){
    GAMEMANAGER.root->RemoveChild(_ownerNode);
    auto it = std::find(ENEMIESMANAGER._enemies.begin(), ENEMIESMANAGER._enemies.end(), shared_from_this());
    if (it != ENEMIESMANAGER._enemies.end()) {
        *it = nullptr;
    }
}

void Enemy::TakeDamage(int amount)
{
    _hp -= amount;

    std::regex pattern("^Particle\\d+$");
    shared_ptr<ParticleGenerator> particleDead;
    auto rootChildren = NODESMANAGER.getNodeByName("root")->getChildren();
    for (const auto& child : rootChildren) {
        if(std::regex_match(child->_name, pattern))
        {
            auto particleGenerators = child->GetAllComponents<ParticleGenerator>();
            for (const auto& generator : particleGenerators) {
                if (generator->particleType == "antShot" && generator->object == this->GetOwnerNode()) generator->SpawnParticles();
                if (generator->particleType == "antDie" && generator->object == this->GetOwnerNode()) particleDead = generator;
            }
        }
    }

    if(_hp <= 0)
    {
        if(particleDead != nullptr) {
            particleDead->SpawnParticles();
            particleDead->toDelete = true;
        }

        auto anim = _ownerNode->GetComponent<Animation>();
        if (anim != nullptr)
        {
            anim->_enemyState = DEAD;
        }
    }
}

void Enemy::AttackDome(){

    if(!_isAtWalls) return;

    if(_attackTimer >= _attackFrequency){
        RESOURCEMANAGER.GetSoundByID(15)->PlaySoundSim(_ownerNode);
        DOMEMANAGER.TakeDamage(_damage);
        //std::cout << "DOME HP: " << DOMEMANAGER.hp << std::endl;
        //std::cout << "ATTACKED DOME FOR " << _damage << std::endl;
        _attackTimer = 0;
    }
    else{
        _attackTimer += TIME.GetDeltaTime();
    }
}

//Just like update but connected to EnemiesManager
void Enemy::EnemyAI()
{
    auto anim = _ownerNode->GetComponent<Animation>();
    if (anim->_enemyState == DEAD && anim->_toDelete)
    {
        Die();
        return;
    }

    if(anim->_enemyState != DEAD)
    {
        AttackDome();
        WalkToDestination();
        Component::Update();
    }
}

void Enemy::setUp()
{
    switch (_enemyType) {
        case ANT:
            _walkingSpeed = 8;
            _hp = 100;
            _damage = 5;
            break;

        case BEETLE:
            _walkingSpeed = 4;
            _hp = 200;
            _damage = 10;
            break;

        case WASP:
            _walkingSpeed = 5;
            _hp = 150;
            _damage = 7;
            break;
    }
}