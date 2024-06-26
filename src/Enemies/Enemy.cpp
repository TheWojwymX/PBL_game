#include "Enemy.h"
#include "Core/Time.h"
#include "EnemiesManager.h"
#include "Managers/DomeManager.h"

Enemy::Enemy() {
    _type = ComponentType::ENEMYAI;
    InitSounds();
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

    if (_isAtWalls) return;

    if (destination != nullptr) {
        _destinationVector = *destination;
    }

    if (glm::distance(_destinationVector, _ownerTransform->GetPosition()) > _distanceToStop && !_shouldStay) {

        _slalomTime += TIME.GetDeltaTime();
        float sideMovement = sin(_slalomTime * _slalomFrequency) * _slalomAmplitude;
        glm::vec3 sideVector = glm::normalize(glm::cross(_ownerTransform->GetRotation() * glm::vec3(0, 1, 0), _ownerTransform->GetRotation() * glm::vec3(0, 0, -1))) * sideMovement;

        glm::vec3 currentPos = _ownerTransform->GetPosition();
        glm::vec3 forwardMovement = Transform::MoveTowards(currentPos, _destinationVector, _speed * TIME.GetDeltaTime()) - currentPos;
        glm::vec3 effectiveMovement;

        effectiveMovement = forwardMovement + sideVector * TIME.GetDeltaTime();

        effectiveMovement.y = 0.0f;

        _ownerTransform->LookAt(glm::normalize(effectiveMovement));
        _ownerTransform->SetPosition(currentPos + effectiveMovement);
    }
}

void Enemy::Die()
{
    GAMEMANAGER.root->RemoveChild(_ownerNode);
    ENEMIESMANAGER.RemoveFromEnemies(shared_from_this());
}

void Enemy::TakeDamage(float amount)
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
                if ((generator->particleType == "antDie" || generator->particleType == "waspDie" || generator->particleType == "beetleDie") && generator->object == this->GetOwnerNode()) particleDead = generator;
            }
        }
    }

    if(_hp <= 0)
    {
        if (_enemyType == WASP)
        {
            Die();
        }
        
        if(particleDead != nullptr) {
            particleDead->SpawnParticles();
            particleDead->toDelete = true;
        }

        ENEMIESMANAGER.removeAttackFromGUI(shared_from_this());

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
        _attackSounds[_enemyType]->PlaySoundSim(_ownerNode);
       
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

    if(anim->_enemyState != DEAD && anim->_enemyState != SPAWN)
    {
        AttackDome();
        WalkToDestination();
        Component::Update();
    }
}

void Enemy::SetStats(EnemyType type, float speed, float hp, float damage, float attackFrequency, float size)
{
    _enemyType = type;
    _speed = speed;
    _hp = hp;
    _damage = damage;
    _attackFrequency = attackFrequency;
    _size = size;
}

void Enemy::InitSounds()
{
    _attackSounds.push_back(RESOURCEMANAGER.GetSoundByName("atak1"));
    _attackSounds.push_back(RESOURCEMANAGER.GetSoundByName("BeetleAttack"));
    _attackSounds.push_back(RESOURCEMANAGER.GetSoundByName("WaspAttack"));
}
