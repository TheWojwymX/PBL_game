#include "Turret.h"
#include "Core/Time.h"
#include "Core/Input.h"
#include "Managers/GameManager.h"

Turret::Turret(): _soundRand(10,0,1){
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

void Turret::SetUp(glm::vec4 stats)
{
    _fireRate = stats.x;
    _damage = stats.y;
    _sideRange = stats.z;
    _forwardRange = stats.w;
    _timer = _fireRate;
}

void Turret::Upgrade(glm::vec4 values)
{
    _damage += values.x;
    _fireRate += values.y;
    _sideRange += values.z;
    _forwardRange += values.w;
    _upgradeLevel++;

    UpdateModel();
    UpdateTooltip();
    CalculateRangePositions();
}

std::pair<int, float> Turret::GetSound() {
    if (_sounds.empty()) {
        throw std::runtime_error("No sounds available"); // Handle case where _sound is empty
    }

    if (_sounds.size() == 1) return _sounds[0];

    return _sounds[_soundRand.GetRandomInt()];
}

void Turret::HandleSpawn()
{
    if(!getInitialRotation == true)
    {
        _initialRotation = _ownerTransform->GetRotation();
        getInitialRotation = true;
    }

    if (_isFlying) {
        auto particleGenerator = _flare->GetComponent<ParticleGenerator>();

        if (glm::distance(GetOwnerPosition(), _finalPosition) <= 0.1)
        {
            _isFlying = false;
            _ownerTransform->SetRotation(_finalRotation);
            _ownerTransform->SetPosition(_finalPosition);

            UpdateModel();
            CalculateRangePositions();
            SetSound();

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
            texture = "res/Particle/Tooltips/minigun_lvl2.png";
            break;
        case 2:
            modelName = "Turret_Minigun_Level3";
            texture = "res/Particle/Tooltips/minigun_lvl3.png";
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
            texture = "res/Particle/Tooltips/sniper_lvl2.png";
            break;
        case 2:
            modelName = "Turret_Sniper_Level3";
            texture = "res/Particle/Tooltips/sniper_lvl3.png";
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
            texture = "res/Particle/Tooltips/rifle_lvl2.png";
            break;
        case 2:
            modelName = "Turret_Rifle_Level3";
            texture = "res/Particle/Tooltips/rifle_lvl3.png";
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

void Turret::SetSound() {

    switch (_turretType) {
    case MINIGUN:
        _sounds = { {7, 0.1f}, {8, 0.1f} };
        break;

    case SNIPER:
        _sounds = { {9, 0.1f} };
        break;

    case RIFLE:
        _sounds = { {7, 0.1f}, {7, 0.1f} };
        break;

    default:
        std::cerr << "Unknown turret type" << std::endl;
        return;
    }
}

void Turret::CalculateRangePositions() {
    auto rangeNodes = _ownerNode->getChildren();
    for (const auto& node : rangeNodes) {
        if (node != nullptr) {
            // Get the unique XZ vertices from the model
            std::vector<glm::vec3> uniqueVerticesXZ = RESOURCEMANAGER.GetModelByName("RangeIndicatorReal")->GetUniqueVerticesXZ();

            // Check if there are vertices to transform
            if (!uniqueVerticesXZ.empty()) {
                // Apply the GlobalCTM to the vertices
                node->GetTransform()->SetScale(glm::vec3(_sideRange, 0.2f, _forwardRange));
                glm::mat4 globalCTM = node->GetTransform()->GetGlobalCTM();
                std::vector<glm::vec3> transformedVertices = Transform::ApplyTransformation(uniqueVerticesXZ, globalCTM);

                // Assign the transformed vertices to the turret's range positions
                if (transformedVertices.size() >= 4) {
                    _turretRangePositions[0] = transformedVertices[0];
                    _turretRangePositions[1] = transformedVertices[1];
                    _turretRangePositions[2] = transformedVertices[2];
                    _turretRangePositions[3] = transformedVertices[3];

                }
                else {
                    std::cerr << "Error: Not enough transformed vertices for turret range positions.\n";
                }
            }
        }
    }
}

void Turret::UpdateTooltip() {
    auto generators = _ownerNode->GetAllComponents<ParticleGenerator>();
    for (const auto& generator : generators){
        if(generator->particleType == "tooltipParticle") generator->texture = Texture2D::loadTextureFromFile(texture, true);
    }
}