#include "Turret.h"
#include "Core/Time.h"
#include "Core/Input.h"
#include "Managers/GameManager.h"

Turret::Turret() {
    _type = ComponentType::TURRET;
}

nlohmann::json Turret::Serialize() {
    nlohmann::json data;

    data["turretRangePositions"] = {
            {"position0", {_turretRangePositions[0].x, _turretRangePositions[0].y, _turretRangePositions[0].z}},
            {"position1", {_turretRangePositions[1].x, _turretRangePositions[1].y, _turretRangePositions[1].z}},
            {"position2", {_turretRangePositions[2].x, _turretRangePositions[2].y, _turretRangePositions[2].z}},
            {"position3", {_turretRangePositions[3].x, _turretRangePositions[3].y, _turretRangePositions[3].z}},
    };

    return data;
}

void Turret::Deserialize(const nlohmann::json &jsonData) {

    if (jsonData.contains("turretRangePositions")) {
        auto turretRangePositions = jsonData["turretRangePositions"];

        auto position0 = turretRangePositions["position0"];
        auto position1 = turretRangePositions["position1"];
        auto position2 = turretRangePositions["position2"];
        auto position3 = turretRangePositions["position3"];

        _turretRangePositions[0] = glm::vec3(position0[0], position0[1], position0[2]);
        _turretRangePositions[1] = glm::vec3(position1[0], position1[1], position1[2]);
        _turretRangePositions[2] = glm::vec3(position2[0], position2[1], position2[2]);
        _turretRangePositions[3] = glm::vec3(position3[0], position3[1], position3[2]);
    }

    Component::Deserialize(jsonData);
}

bool stop = false;

void Turret::Update() {

    if(GAMEMANAGER._paused) return;

    HandleSpawn();
}

void Turret::setUp()
{
    switch(_turretType)
    {
        case MINIGUN:
            _fireRate = 0.5;
            _damage = 1;
            break;
        case SNIPER:
            _fireRate = 2;
            _damage = 10;
            break;
        case RIFLE:
            _fireRate = 1;
            _damage = 1;
            break;
    }
}

void Turret::Upgrade(glm::vec2 values)
{
    _damage += values.x;
    _fireRate += values.y;
    _upgradeLevel++;
    UpdateModel();
}

void Turret::HandleSpawn()
{
    if (_isFlying) {
        auto particleGenerator = _flare->GetComponent<ParticleGenerator>();

        if (glm::distance(GetOwnerPosition(), _finalPosition) <= 0.1)
        {
            _isFlying = false;
            _ownerTransform->SetRotation(_finalRotation);
            _ownerTransform->SetPosition(_finalPosition);

            UpdateModel();

            _flare->GetComponent<MeshRenderer>()->_disableModel = true;
            particleGenerator->toDelete = true;
            return;
        }

        particleGenerator->SpawnParticles();

        _ownerTransform->SetPosition(glm::vec3(GetOwnerPosition().x, GetOwnerPosition().y - TIME.GetDeltaTime() * _flyingSpeed, GetOwnerPosition().z));

        if (_swingDirection == 1) {
            _swingTimer += TIME.GetDeltaTime();
            if (_swingTimer >= _rightMax) {
                _swingTimer = _rightMax;
                _swingDirection = 2;
            }
        }
        else if (_swingDirection == 2) {
            _swingTimer -= TIME.GetDeltaTime();
            if (_swingTimer <= _leftMax) {
                _swingTimer = _leftMax;
                _swingDirection = 1;
            }
        }

        glm::quat swingRotation = glm::angleAxis(0.3f * _swingTimer, glm::vec3(0, 0, 1));
        glm::quat newRotation = _finalRotation * swingRotation;
        _ownerTransform->SetRotation(newRotation);
    }
}

void Turret::UpdateModel() {
    auto meshRenderer = _ownerNode->GetComponent<MeshRenderer>();
    if (!meshRenderer) {
        std::cerr << "MeshRenderer component not found!" << std::endl;
        return;
    }

    std::string modelName;

    switch (_turretType) {
    case MINIGUN:
        switch (_upgradeLevel) {
        case 0:
            modelName = "Turret_Minigun_Level1";
            break;
        case 1:
            modelName = "Turret_Minigun_Level2";
            break;
        case 2:
            modelName = "Turret_Minigun_Level3";
            break;
        default:
            std::cerr << "Invalid upgrade level for Minigun" << std::endl;
            return;
        }
        break;

    case SNIPER:
        switch (_upgradeLevel) {
        case 0:
            modelName = "Turret_Sniper_Level1";
            break;
        case 1:
            modelName = "Turret_Sniper_Level2";
            break;
        case 2:
            modelName = "Turret_Sniper_Level3";
            break;
        default:
            std::cerr << "Invalid upgrade level for Sniper" << std::endl;
            return;
        }
        break;

    case RIFLE:
        switch (_upgradeLevel) {
        case 0:
            modelName = "Turret_Rifle_Level1";
            break;
        case 1:
            modelName = "Turret_Rifle_Level2";
            break;
        case 2:
            modelName = "Turret_Rifle_Level3";
            break;
        default:
            std::cerr << "Invalid upgrade level for Rifle" << std::endl;
            return;
        }
        break;

    default:
        std::cerr << "Unknown turret type" << std::endl;
        return;
    }

    meshRenderer->_model = RESOURCEMANAGER.GetModelByName(modelName);
}

