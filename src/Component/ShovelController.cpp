//
// Created by Jacek on 30.05.2024.
//

#include "ShovelController.h"

ShovelController::ShovelController() {
    _type = ComponentType::SHOVELCONTROLLER;
}

nlohmann::json ShovelController::Serialize() {
    return Component::Serialize();
}

void ShovelController::Deserialize(const nlohmann::json &jsonData) {
    Component::Deserialize(jsonData);
}

void ShovelController::Init() {
    _playerNode = NODESMANAGER.getNodeByName("player");
    _shovelNode = NODESMANAGER.getNodeByName("Shovel");
    _hideY = GAMEMANAGER._groundLevel - 0.8f;
    Component::Init();
}

void ShovelController::Update() {
    Component::Update();
}

void ShovelController::RealUpdate() {

    float playerY = _playerNode->GetTransform()->GetPosition().y;

    if (playerY >= _hideY && !_isHidden && !_playShowAnim) {
        _playHideAnim = true;
    } else if (playerY < _hideY && _isHidden && !_playHideAnim) {
        _playShowAnim = true;
    }

    if (_playDigAnim) {
        PlayDigAnimation();
    }

    if (_playHideAnim) {
        PlayHideShovel();
    }
    if (_playShowAnim) {
        PlayShowShovel();
    }

    SetPosAndRot();
}

void ShovelController::PlayHideShovel() {
    if (_visibilityAnimationTimer < _visibilityAnimationTime) {
        _visibilityAnimationTimer += TIME.GetDeltaTime();
        if (_visibilityAnimationTimer < _visibilityAnimationTime) {
            float t = glm::clamp(_visibilityAnimationTimer / _visibilityAnimationTime, 0.0f, 1.0f);
            _actualVisibilityOffset = glm::mix(_actualVisibilityOffset, _visibilityOffset, t);
        } else {
            _actualVisibilityOffset = _visibilityOffset;
            _visibilityAnimationTimer = 0;
            _playHideAnim = false;
            _isHidden = true;
        }
    }
}

void ShovelController::PlayShowShovel() {
    if (_visibilityAnimationTimer < _visibilityAnimationTime) {
        _visibilityAnimationTimer += TIME.GetDeltaTime();
        if (_visibilityAnimationTimer < _visibilityAnimationTime) {
            float t = glm::clamp(_visibilityAnimationTimer / _visibilityAnimationTime, 0.0f, 1.0f);
            _actualVisibilityOffset = glm::mix(_actualVisibilityOffset, 0.0f, t);
        } else {
            _actualVisibilityOffset = 0;
            _visibilityAnimationTimer = 0;
            _playShowAnim = false;
            _isHidden = false;
        }
    }
}

void ShovelController::PlayDigAnimation() {

    if (_digAnimationTimer < _digAnimationTime) {
        _digAnimationTimer += TIME.GetDeltaTime();
        if (_digAnimationTimer < _digAnimationTime / 2) {
            float t = glm::clamp(_digAnimationTimer / _digAnimationTime / 2, 0.0f, 1.0f);
            _actualDegree = glm::mix(_actualDegree, _maxDegree, t);
        } else {
            float t = glm::clamp(_digAnimationTimer / _digAnimationTime / 2, 0.0f, 1.0f);
            _actualDegree = glm::mix(_actualDegree, _minDegree, t);
        }

    } else {
        _digAnimationTimer = 0;
        _actualDegree = 0;
        _playDigAnim = false;
    }
}

void ShovelController::SetPosAndRot() {
    glm::vec3 forwardVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetFrontVector());
    glm::vec3 rightVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetRightVector());
    glm::vec3 upVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetUpVector());
    auto cameraPos = _playerNode->GetComponent<Camera>()->GetPosition();

    glm::vec3 shovelPosition = cameraPos + forwardVector * _forwardOffset + rightVector * _horizontalOffset + upVector * _verticalOffset;

    glm::vec3 additionalOffset = upVector * -_actualVisibilityOffset;
    shovelPosition += additionalOffset;
    _shovelNode->GetTransform()->SetPosition(shovelPosition);

    glm::mat4 rotationMatrix(1.0f);
    rotationMatrix[0] = glm::vec4(rightVector, 0.0f);  // x-axis
    rotationMatrix[1] = glm::vec4(upVector, 0.0f);     // y-axis
    rotationMatrix[2] = glm::vec4(-forwardVector, 0.0f); // z-axis (negative because camera looks towards -z)

    _shovelNode->GetTransform()->SetRotation(glm::quat_cast(rotationMatrix));

    glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(_ownerTransform->GetRotation()));
    _shovelNode->GetTransform()->SetRotation(glm::vec3(eulerRotation.x - _actualDegree, eulerRotation.y, eulerRotation.z));
}