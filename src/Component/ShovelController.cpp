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
    //SetPosAndRot();
    Component::Update();
}

void ShovelController::RealUpdate(){
    SetPosAndRot();
}

void ShovelController::SetPosAndRot(){
    glm::vec3 forwardVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetFrontVector());
    glm::vec3 rightVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetRightVector());
    glm::vec3 upVector = glm::normalize(_playerNode->GetComponent<Camera>()->GetUpVector());
    auto cameraPos = _playerNode->GetComponent<Camera>()->GetPosition();

    float forwardOffset = 2.0f;  // Dystans przed kamerą
    float horizontalOffset = 1.3f;  // Dystans w prawo od kamery
    float verticalOffset = -1.0f;  // Dystans w dół od kamery

    glm::vec3 shovelPosition = cameraPos + forwardVector * forwardOffset + rightVector * horizontalOffset + upVector * verticalOffset;
    _shovelNode->GetTransform()->SetPosition(shovelPosition);

    glm::mat4 rotationMatrix(1.0f);
    rotationMatrix[0] = glm::vec4(rightVector, 0.0f);  // x-axis
    rotationMatrix[1] = glm::vec4(upVector, 0.0f);     // y-axis
    rotationMatrix[2] = glm::vec4(-forwardVector, 0.0f); // z-axis (negative because camera looks towards -z)

    _shovelNode->GetTransform()->SetRotation(glm::quat_cast(rotationMatrix));

}