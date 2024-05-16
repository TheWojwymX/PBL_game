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
        if(COMPONENTSMANAGER.GetComponents()[i] != nullptr && COMPONENTSMANAGER.GetComponents()[i]->_type == ComponentType::TURRET){
            _turrets.push_back(std::dynamic_pointer_cast<Turret>(COMPONENTSMANAGER.GetComponents()[i]));
        }
    }
    PrepareBlueprintTurret();
}

void TurretsManager::Update() {
    if(Input::Instance().IsKeyPressed(76)) {
        _shouldEnableBlueprintTurret = !_shouldEnableBlueprintTurret;
        NODESMANAGER.getNodeByName("BlueprintTurret")->GetComponent<MeshRenderer>()->SetEnabled(!NODESMANAGER.getNodeByName("BlueprintTurret")->GetComponent<MeshRenderer>()->IsEnabled());
        _isInBlueprintMode = !_isInBlueprintMode;
    }

    if(INPUT.IsMousePressed(0) && _isInBlueprintMode && !IsTooCloseToTurret(NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition())){
        SpawnTurret();
    }

    UpdateBlueprintTurret();
}

bool TurretsManager::IsTooCloseToTurret(glm::vec3 pos){
    for(int i = 0; i < _turrets.size(); i++){

        if(_turrets[i] == nullptr) continue;

        if(glm::distance(pos, _turrets[i]->_finalPosition) < _distanceToAnotherTurret){
            return true;
        }
    }
    return false;
}

void TurretsManager::SpawnTurret(){
    std::cout << "zespawniono" << std::endl;
    std::string nameOfTurret = "Turret" + to_string(_turrets.size() + 1);
    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), nameOfTurret);

    auto newMeshRenderer = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    newMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("Turret_Parachute");
    newMeshRenderer->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
    newMeshRenderer->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    newMeshRenderer->Initiate();
    NODESMANAGER.getNodeByName(nameOfTurret)->AddComponent(newMeshRenderer);

    auto newTurret = COMPONENTSMANAGER.CreateComponent<Turret>();
    newTurret->Initiate();
    newTurret->_isFlying = true;
    newTurret->_finalRotation = NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetRotation();
    newTurret->_finalPosition = NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition();

    glm::vec3 BPPosition = NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition();
    glm::vec3 upVec = glm::vec3(0, 1, 0);
    glm::vec3 forwardVec = NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetRotation() * glm::vec3(0, 0, 1);
    glm::vec3 backwardVec = -forwardVec;
    glm::vec3 leftVec = glm::normalize(glm::cross(upVec, forwardVec));
    glm::vec3 rightVec = -leftVec;

    glm::vec3 pos0 = BPPosition + leftVec * _sideRange + backwardVec * _backRange;
    newTurret->_turretRangePositions.push_back(pos0);

    glm::vec3 pos1 = BPPosition + forwardVec * _forwardRange + leftVec * _sideRange;
    newTurret->_turretRangePositions.push_back(pos1);

    glm::vec3 pos2 = BPPosition + forwardVec * _forwardRange + rightVec * _sideRange;
    newTurret->_turretRangePositions.push_back(pos2);

    glm::vec3 pos3 = BPPosition + rightVec * _sideRange + backwardVec * _backRange;
    newTurret->_turretRangePositions.push_back(pos3);

/*    std::cout << "_turretRangePositions[0]: x=" << newTurret->_turretRangePositions[0].x
              << ", y=" << newTurret->_turretRangePositions[0].y
              << ", z=" << newTurret->_turretRangePositions[0].z << std::endl;

    std::cout << "_turretRangePositions[1]: x=" << newTurret->_turretRangePositions[1].x
              << ", y=" << newTurret->_turretRangePositions[1].y
              << ", z=" << newTurret->_turretRangePositions[1].z << std::endl;

    std::cout << "_turretRangePositions[2]: x=" << newTurret->_turretRangePositions[2].x
              << ", y=" << newTurret->_turretRangePositions[2].y
              << ", z=" << newTurret->_turretRangePositions[2].z << std::endl;

    std::cout << "_turretRangePositions[3]: x=" << newTurret->_turretRangePositions[3].x
              << ", y=" << newTurret->_turretRangePositions[3].y
              << ", z=" << newTurret->_turretRangePositions[3].z << std::endl;*/


    NODESMANAGER.getNodeByName(nameOfTurret)->AddComponent(newTurret);

    NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->SetPosition(glm::vec3(NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition().x,
                                                                                    NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition().y + 10,
                                                                                    NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition().z));
    NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->SetScale(NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetScale());
    NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->SetRotation(NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetRotation());

    _turrets.push_back(NODESMANAGER.getNodeByName(nameOfTurret)->GetComponent<Turret>());
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

    //positions counting
    glm::vec3 BPPosition = NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition();
    glm::vec3 upVec = glm::vec3(0, 1, 0);
    glm::vec3 forwardVec = NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetRotation() * glm::vec3(0, 0, 1);
    glm::vec3 backwardVec = -forwardVec;
    glm::vec3 leftVec = glm::normalize(glm::cross(upVec, forwardVec));
    glm::vec3 rightVec = -leftVec;

    glm::vec3 pos0 = BPPosition + leftVec * _sideRange + backwardVec * _backRange;
    glm::vec3 pos1 = BPPosition + forwardVec * _forwardRange + leftVec * _sideRange;
    glm::vec3 pos2 = BPPosition + forwardVec * _forwardRange + rightVec * _sideRange;
    glm::vec3 pos3 = BPPosition + rightVec * _sideRange + backwardVec * _backRange;
}