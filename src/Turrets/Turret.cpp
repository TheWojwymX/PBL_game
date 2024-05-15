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
    return data;
}

void Turret::Deserialize(const nlohmann::json &jsonData) {
    Component::Deserialize(jsonData);
}

bool stop = false;

void Turret::Update() {

    if(GAMEMANAGER._paused) return;

    if(_isFlying){
        if(glm::distance(GetOwnerPosition(), _finalPosition) <= 0.1){
            _isFlying = false;
            _ownerTransform->SetRotation(_finalRotation);
            _ownerTransform->SetPosition(_finalPosition);
            _ownerNode->GetComponent<MeshRenderer>()->_model = RESOURCEMANAGER.GetModelByName("Turret_Gunner_Level1");
            return;
           }

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
