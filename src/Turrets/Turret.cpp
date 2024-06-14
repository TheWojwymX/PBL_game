//
// Created by Jacek on 13.05.2024.
//

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

    if(_isFlying){
        auto particleGenerator = _flare->GetComponent<ParticleGenerator>();

        if(glm::distance(GetOwnerPosition(), _finalPosition) <= 0.1)
        {
            _isFlying = false;
            _ownerTransform->SetRotation(_finalRotation);
            _ownerTransform->SetPosition(_finalPosition);

            auto model =  _ownerNode->GetComponent<MeshRenderer>()->_model;
            switch(_turretType)
            {
                case MINIGUN:
                    model = RESOURCEMANAGER.GetModelByName("Turret_Minigun_Level1");
                    break;

                case SNIPER:
                    model = RESOURCEMANAGER.GetModelByName("Turret_Sniper_Level1");
                    break;

                case RIFLE:
                    model = RESOURCEMANAGER.GetModelByName("Turret_Rifle_Level1");
                    break;
            }

            std::cout << "kiedy sie wykona";
            _ownerNode->GetComponent<MeshRenderer>()->_model = model;
            _flare->GetComponent<MeshRenderer>()->_disableModel = true;
            particleGenerator->toDelete = true;
            return;
           }

        particleGenerator->SpawnParticles();

        _ownerTransform->SetPosition(glm::vec3(GetOwnerPosition().x, GetOwnerPosition().y - TIME.GetDeltaTime() * _flyingSpeed, GetOwnerPosition().z));

        if(_swingDirection == 1){
            _swingTimer += TIME.GetDeltaTime();
            if(_swingTimer >= _rightMax){
                _swingTimer = _rightMax;
                _swingDirection = 2;
            }
        }
        else if(_swingDirection == 2){
            _swingTimer -= TIME.GetDeltaTime();
            if(_swingTimer <= _leftMax){
                _swingTimer = _leftMax;
                _swingDirection = 1;
            }
        }

        glm::quat swingRotation = glm::angleAxis(0.3f * _swingTimer, glm::vec3(0, 0, 1));
        glm::quat newRotation = _finalRotation * swingRotation;
        _ownerTransform->SetRotation(newRotation);
    }

    Component::Update();
}

void Turret::setUp()
{
    switch(_turretType)
    {
        case MINIGUN:
            _fireRate = 0.5;
            _damage = 35;
            break;
        case SNIPER:
            _fireRate = 0.5;
            _damage = 35;
            break;
        case RIFLE:
            _fireRate = 0.5;
            _damage = 35;
            break;
    }
}

