//
// Created by Jacek on 13.05.2024.
//

#include "TurretsManager.h"

TurretsManager &TurretsManager::getInstance() {
    static TurretsManager instance;
    return instance;
}

void TurretsManager::Init() {
    for(int i = 0; i < COMPONENTSMANAGER.GetComponents().size(); i++){
        if(COMPONENTSMANAGER.GetComponents()[i] != nullptr && COMPONENTSMANAGER.GetComponents()[i]->_type == ComponentType::ENEMYAI){
            _turrets.push_back(std::dynamic_pointer_cast<Turret>(COMPONENTSMANAGER.GetComponents()[i]));
        }
    }
    PrepareBlueprintTurret();
}

void TurretsManager::Update() {
    if(Input::Instance().IsKeyPressed(76)) {
        _shouldEnableBlueprintTurret = !_shouldEnableBlueprintTurret;
        NODESMANAGER.getNodeByName("BlueprintTurret")->GetComponent<MeshRenderer>()->SetEnabled(!NODESMANAGER.getNodeByName("BlueprintTurret")->GetComponent<MeshRenderer>()->IsEnabled());
    }
    UpdateBlueprintTurret();
}

void TurretsManager::PrepareBlueprintTurret() {
    std::string nameOfBlueprintTurret = "BlueprintTurret";
    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), nameOfBlueprintTurret);

    auto newMeshRenderer = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    newMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("Turret_Gunner_Level1");
    newMeshRenderer->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
    newMeshRenderer->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    newMeshRenderer->Initiate();
    NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->AddComponent(newMeshRenderer);
    NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetComponent<MeshRenderer>()->SetEnabled(false);

    //std::cout <<  NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->GetRotation().x << "   " << NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->GetRotation().y << "  " << NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->GetRotation().z << std::endl;
}

void TurretsManager::UpdateBlueprintTurret(){

    if(!_shouldEnableBlueprintTurret) return;

    std::string nameOfBlueprintTurret = "BlueprintTurret";

    glm::vec3 forwardVector = glm::normalize(NODESMANAGER.getNodeByName("player")->GetComponent<Camera>()->GetFrontVector());
    float minDistance = 2.0f;
    glm::vec3 turretPosition = NODESMANAGER.getNodeByName("player")->GetTransform()->GetPosition() + forwardVector * minDistance;
    turretPosition.y = 99.5;
    glm::vec2 playerFlatPosition(NODESMANAGER.getNodeByName("player")->GetTransform()->GetPosition().x, NODESMANAGER.getNodeByName("player")->GetTransform()->GetPosition().z);
    glm::vec2 turretFlatPosition(turretPosition.x, turretPosition.z);
    if (glm::distance(playerFlatPosition, turretFlatPosition) < minDistance) {
        glm::vec2 direction = glm::normalize(turretFlatPosition - playerFlatPosition);
        turretFlatPosition = playerFlatPosition + direction * minDistance;
        turretPosition.x = turretFlatPosition.x;
        turretPosition.z = turretFlatPosition.y;
    }
    NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetTransform()->SetPosition(turretPosition);

    NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetTransform()->SetScale(0.3);

    glm::vec3 lookAtPoint = turretPosition + forwardVector;
    lookAtPoint.y = turretPosition.y;
    glm::vec3 lookDirection = glm::normalize(lookAtPoint - turretPosition);
    NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetTransform()->LookAt(lookDirection);
}