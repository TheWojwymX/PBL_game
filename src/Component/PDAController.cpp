//
// Created by Jacek on 08.06.2024.
//

#include "PDAController.h"
#include "HUD/PageManager.h"

PDAController::PDAController() {
    _type = ComponentType::SHOVELCONTROLLER;
}

nlohmann::json PDAController::Serialize() {
    return Component::Serialize();
}

void PDAController::Deserialize(const nlohmann::json &jsonData) {
    Component::Deserialize(jsonData);
}

void PDAController::Init() {
    _playerNode = NODESMANAGER.getNodeByName("player");
    _PDANode = NODESMANAGER.getNodeByName("PDA");
    _PDAAntennaNode = NODESMANAGER.getNodeByName("PDAAntenna");
    _playHideAnim = true;
    Component::Init();
}

void PDAController::Update() {
    Component::Update();
}

void PDAController::RealUpdate() {


    if(PAGEMANAGER._isInPage == false && !_isHidden){
        PlayHidePDA();
    }

    if (_playHideAnim) {
        PlayHidePDA();
    }
    if (_playShowAnim) {
        PlayShowPDA();
    }
    if (_playShowAntennaAnim) {
        PlayShowAntenna();
    }

    SetPosAndRot();
}

void PDAController::PlayShowAntenna(){
    //std::cout << "aktualny offset: " << _antennaActualYoffset << "    docelowy: " << _antennaYoffset << std::endl;

    if(_antennaActualYoffset >= _antennaYoffset){
        _antennaActualYoffset -= 3 * TIME.GetDeltaTime();
    }
    else{
        _playShowAntennaAnim = false;
    }
}

void PDAController::HideImmediately(){
    _playShowAnim = false;
    _playShowAntennaAnim = false;
    _actualVisibilityOffset = _visibilityOffset;
    _visibilityAnimationTimer = 0;
    _playHideAnim = false;
    _isHidden = true;
    _antennaActualYoffset = 0;
}

void PDAController::PlayHidePDA() {
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
            _antennaActualYoffset = 0;
        }
    }
}

void PDAController::PlayShowPDA() {
    _isHidden = false;
    if (_visibilityAnimationTimer < _visibilityAnimationTime) {
        _visibilityAnimationTimer += TIME.GetDeltaTime();
        if (_visibilityAnimationTimer < _visibilityAnimationTime) {
            if(_visibilityAnimationTimer >= 0.5 * _visibilityAnimationTime && _playShowAntennaAnim == false){
                _playShowAntennaAnim = true;
            }
            float t = glm::clamp(_visibilityAnimationTimer / _visibilityAnimationTime, 0.0f, 1.0f);
            _actualVisibilityOffset = glm::mix(_actualVisibilityOffset, 0.0f, t);
        } else {
            _actualVisibilityOffset = 0;
            _visibilityAnimationTimer = 0;
            _playShowAnim = false;
        }
    }
}

void PDAController::SetPosAndRot() {
    glm::vec3 forwardVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetFrontVector());
    glm::vec3 rightVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetRightVector());
    glm::vec3 upVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetUpVector());
    auto cameraPos = _playerNode->GetComponent<Camera>()->GetPosition();

    glm::vec3 PDAPosition = cameraPos + forwardVector * _forwardOffset + rightVector * _horizontalOffset + upVector * _verticalOffset;

    float PDAAntennaForwardOffset = _forwardOffset;
    float PDAAntennaHorizontalOffset = _horizontalOffset;
    float PDAAntennaVerticalOffset = _verticalOffset - 1.1 - _antennaActualYoffset;

    glm::vec3 PDAAntennePosition = cameraPos + forwardVector * PDAAntennaForwardOffset + rightVector * PDAAntennaHorizontalOffset + upVector * PDAAntennaVerticalOffset;

    glm::vec3 additionalOffset = upVector * -_actualVisibilityOffset;

    PDAPosition += additionalOffset;
    PDAAntennePosition += additionalOffset;

    _PDANode->GetTransform()->SetPosition(PDAPosition);
    _PDAAntennaNode->GetTransform()->SetPosition(PDAAntennePosition);

    glm::mat4 rotationMatrix(1.0f);
    rotationMatrix[0] = glm::vec4(rightVector, 0.0f);  // x-axis
    rotationMatrix[1] = glm::vec4(upVector, 0.0f);     // y-axis
    rotationMatrix[2] = glm::vec4(-forwardVector, 0.0f); // z-axis (negative because camera looks towards -z)

    _PDANode->GetTransform()->SetRotation(glm::quat_cast(rotationMatrix));
    _PDAAntennaNode->GetTransform()->SetRotation(glm::quat_cast(rotationMatrix));

    glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(_ownerTransform->GetRotation()));
    _PDANode->GetTransform()->SetRotation(glm::vec3(eulerRotation.x - 20, eulerRotation.y, eulerRotation.z));
    _PDAAntennaNode->GetTransform()->SetRotation(glm::vec3(eulerRotation.x - 20, eulerRotation.y, eulerRotation.z));
}