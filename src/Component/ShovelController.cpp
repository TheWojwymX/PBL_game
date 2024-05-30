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
    Component::Init();
}

void ShovelController::Update() {
    //_ownerTransform->SetPosition(_playerNode->GetTransform()->GetPosition());
    Component::Update();
}