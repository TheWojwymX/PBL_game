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
    Component::Init();
}

void ShovelController::Update() {
    Component::Update();
}

void ShovelController::RealUpdate(){
    if(_playAnim){
        PlayDigAnimation();
    }
    SetPosAndRot();
}

void ShovelController::PlayDigAnimation(){

    if(_animationTimer < _animationTime){
        _animationTimer += TIME.GetDeltaTime();
        if(_animationTimer < _animationTime/2){
            float t = glm::clamp(_animationTimer / _animationTime/2, 0.0f, 1.0f);
            _actualDegree = glm::mix(_actualDegree, _maxDegree, t);
        }else{
            float t = glm::clamp(_animationTimer / _animationTime/2, 0.0f, 1.0f);
            _actualDegree = glm::mix(_actualDegree, _minDegree, t);
        }

    }
    else{
        _animationTimer = 0;
        _actualDegree = 0;
        _playAnim = false;
    }


}

void ShovelController::SetPosAndRot(){
    glm::vec3 forwardVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetFrontVector());
    glm::vec3 rightVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetRightVector());
    glm::vec3 upVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetUpVector());
    auto cameraPos = _playerNode->GetComponent<Camera>()->GetPosition();

    glm::vec3 shovelPosition = cameraPos + forwardVector * _forwardOffset + rightVector * _horizontalOffset + upVector * _verticalOffset;
    _shovelNode->GetTransform()->SetPosition(shovelPosition);

    glm::mat4 rotationMatrix(1.0f);
    rotationMatrix[0] = glm::vec4(rightVector, 0.0f);  // x-axis
    rotationMatrix[1] = glm::vec4(upVector, 0.0f);     // y-axis
    rotationMatrix[2] = glm::vec4(-forwardVector, 0.0f); // z-axis (negative because camera looks towards -z)

    _shovelNode->GetTransform()->SetRotation(glm::quat_cast(rotationMatrix));

    glm::vec3 eulerRotation = glm::degrees(glm::eulerAngles(_ownerTransform->GetRotation()));

    _shovelNode->GetTransform()->SetRotation(glm::vec3(eulerRotation.x - _actualDegree, eulerRotation.y, eulerRotation.z));
}