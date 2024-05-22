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

    UpdateBlueprintTurret();

    if(Input::Instance().IsKeyPressed(76) && !_isPlayerInMovingMode) {
        _shouldEnableBlueprintTurret = !_shouldEnableBlueprintTurret;
        NODESMANAGER.getNodeByName("BlueprintTurret")->GetComponent<MeshRenderer>()->SetEnabled(!NODESMANAGER.getNodeByName("BlueprintTurret")->GetComponent<MeshRenderer>()->IsEnabled());
        //NODESMANAGER.getNodeByName("BlueprintRange")->GetComponent<MeshRenderer>()->SetEnabled(!NODESMANAGER.getNodeByName("BlueprintRange")->GetComponent<MeshRenderer>()->IsEnabled());
        _isInBlueprintMode = !_isInBlueprintMode;
    }

    if(INPUT.IsMousePressed(0) && _isInBlueprintMode && !IsTooCloseToTurret(NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition()) && _isPlayerInMovingMode == false){
        SpawnTurret();
    }

    if(INPUT.IsMousePressed(1) && !_isInBlueprintMode && !_isPlayerInMovingMode && RaycastTurrets() >= 0 && !_turrets[RaycastTurrets()]->_isFlying){
        _indexOfMovingTurret = RaycastTurrets();
        _isPlayerInMovingMode = true;
        _turrets[_indexOfMovingTurret]->_isMoving = true;
    }else if(INPUT.IsMousePressed(1) && !_isInBlueprintMode && _isPlayerInMovingMode){
        PlaceMovingTurret();
    }

    MoveTurret();
    CheckEnemiesInRange();
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

    if(GAMEMANAGER._money < _turretCost){
        std::cout << "Brak pieniedzy na dzialko" << std::endl;
        return;
    }
    else{
        GAMEMANAGER._money -= _turretCost;
    }

    //std::cout << "zespawniono" << std::endl;
    std::string nameOfTurret = "Turret" + to_string(_turrets.size() + 1);
    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), nameOfTurret);

    auto newMeshRenderer = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    newMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("Turret_Parachute");
    newMeshRenderer->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
    newMeshRenderer->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    newMeshRenderer->Initiate();
    NODESMANAGER.getNodeByName(nameOfTurret)->AddComponent(newMeshRenderer);

    auto bulletParticles = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"),"turretShot");
    bulletParticles->SetOffset(glm::vec3(0.0f,1.0f,0.0f));
    bulletParticles->object = NODESMANAGER.getNodeByName(nameOfTurret);
    bulletParticles->Init();
    NODESMANAGER.getNodeByName(nameOfTurret)->AddComponent(bulletParticles);

    auto casingParticles = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"),"turretCasing");
    casingParticles->SetOffset(glm::vec3(0.0f,1.0f,0.0f));
    casingParticles->object = NODESMANAGER.getNodeByName(nameOfTurret);
    casingParticles->Init();
    NODESMANAGER.getNodeByName(nameOfTurret)->AddComponent(casingParticles);

    auto newTurret = COMPONENTSMANAGER.CreateComponent<Turret>();
    newTurret->Initiate();
    newTurret->_isFlying = true;
    newTurret->_finalRotation = NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetRotation();
    newTurret->_finalPosition = NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition();

    CalculateRangePositions(newTurret);

    NODESMANAGER.getNodeByName(nameOfTurret)->AddComponent(newTurret);

    NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->SetPosition(glm::vec3(NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition().x,
                                                                                    NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition().y + 10,
                                                                                    NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition().z));
    NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->SetScale(NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetScale());
    NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->SetRotation(NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetRotation());

    _turrets.push_back(NODESMANAGER.getNodeByName(nameOfTurret)->GetComponent<Turret>());
}

void TurretsManager::CalculateRangePositions(shared_ptr<Turret> turret){
    glm::vec3 BPPosition = NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition();
    glm::vec3 upVec = glm::vec3(0, 1, 0);
    glm::vec3 forwardVec = NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetRotation() * glm::vec3(0, 0, 1);
    glm::vec3 backwardVec = -forwardVec;
    glm::vec3 leftVec = glm::normalize(glm::cross(upVec, forwardVec));
    glm::vec3 rightVec = -leftVec;

    glm::vec3 pos0 = BPPosition + leftVec * _sideRange + backwardVec * _backRange;
    turret->_turretRangePositions[0] = pos0;

    glm::vec3 pos1 = BPPosition + forwardVec * _forwardRange + leftVec * _sideRange;
    turret->_turretRangePositions[1] = pos1;

    glm::vec3 pos2 = BPPosition + forwardVec * _forwardRange + rightVec * _sideRange;
    turret->_turretRangePositions[2] = pos2;

    glm::vec3 pos3 = BPPosition + rightVec * _sideRange + backwardVec * _backRange;
    turret->_turretRangePositions[3] = pos3;

/*    std::cout << "_turretRangePositions[0]: x=" << turret->_turretRangePositions[0].x
              << ", y=" << turret->_turretRangePositions[0].y
              << ", z=" << turret->_turretRangePositions[0].z << std::endl;

    std::cout << "_turretRangePositions[1]: x=" << turret->_turretRangePositions[1].x
              << ", y=" << turret->_turretRangePositions[1].y
              << ", z=" << turret->_turretRangePositions[1].z << std::endl;

    std::cout << "_turretRangePositions[2]: x=" << turret->_turretRangePositions[2].x
              << ", y=" << turret->_turretRangePositions[2].y
              << ", z=" << turret->_turretRangePositions[2].z << std::endl;

    std::cout << "_turretRangePositions[3]: x=" << turret->_turretRangePositions[3].x
              << ", y=" << turret->_turretRangePositions[3].y
              << ", z=" << turret->_turretRangePositions[3].z << std::endl;*/
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

/*    std::string nameOfBlueprintRange = "BlueprintRange";
    NODESMANAGER.createNode(NODESMANAGER.getNodeByName(nameOfBlueprintTurret), nameOfBlueprintRange);

    auto newMeshRenderer2 = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    newMeshRenderer2->_model = RESOURCEMANAGER.GetModelByName("cloudModel");
    newMeshRenderer2->_shader = RESOURCEMANAGER.GetShaderByName("turretRangeShader");
    newMeshRenderer2->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    newMeshRenderer2->Initiate();
    NODESMANAGER.getNodeByName(nameOfBlueprintRange)->AddComponent(newMeshRenderer2);
    NODESMANAGER.getNodeByName(nameOfBlueprintRange)->GetComponent<MeshRenderer>()->SetEnabled(false);*/


    //std::cout <<  NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->GetRotation().x << "   " << NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->GetRotation().y << "  " << NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->GetRotation().z << std::endl;
}

void TurretsManager::UpdateBlueprintTurret(){

    //if(!_shouldEnableBlueprintTurret) return;

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

void TurretsManager::CheckEnemiesInRange(){
    for(int i = 0; i < _turrets.size(); i++){

        if(_turrets[i] == nullptr) continue;

        if(_turrets[i]->_isFlying) continue;

        if(_turrets[i]->_isMoving) continue;

        std::vector<shared_ptr<Enemy>> enemiesInRange;

        for(int j = 0; j < ENEMIESMANAGER._enemies.size(); j++){

            if(ENEMIESMANAGER._enemies[j] == nullptr) continue;

            glm::vec3 enemyPos = ENEMIESMANAGER._enemies[j]->GetOwnerPosition();

            if(isPointInRectangle(enemyPos, _turrets[i]->_turretRangePositions)){
                enemiesInRange.push_back(ENEMIESMANAGER._enemies[j]);
            }
        }

        std::shared_ptr<Enemy> closestEnemy;

        if(!enemiesInRange.empty()) {
            closestEnemy = enemiesInRange[0];

            for (int j = 0; j < enemiesInRange.size(); j++) {
                if (glm::distance(_turrets[i]->GetOwnerPosition(),
                                  closestEnemy->GetOwnerPosition()) >
                    glm::distance(_turrets[i]->GetOwnerPosition(),
                                  enemiesInRange[j]->GetOwnerPosition())) {
                    closestEnemy = enemiesInRange[j];
                }
            }

            glm::vec3 turretPosition = _turrets[i]->_ownerNode->GetTransform()->GetPosition();
            glm::vec3 enemyPosition = closestEnemy->GetOwnerPosition();
            enemyPosition.y = turretPosition.y;
            glm::vec3 lookDirection = glm::normalize(enemyPosition - turretPosition);
            _turrets[i]->_ownerNode->GetTransform()->LookAt(lookDirection);

            AttackEnemy(_turrets[i], closestEnemy);

            //std::cout << "najblizej jest " << closestEnemy->_ownerNode->_name << endl;
        }
        else{
            Reload(_turrets[i]);
        }
    }
}

void TurretsManager::Reload(const shared_ptr<Turret>& turret){
    if(turret->_timer < turret->_fireRate){
        turret->_timer += TIME.GetDeltaTime();
    }
}

void TurretsManager::AttackEnemy(const shared_ptr<Turret>& turret, const shared_ptr<Enemy>& enemy){
    if(turret->_timer < turret->_fireRate){
        turret->_timer += TIME.GetDeltaTime();
    }
    else{
        turret->_timer = 0.0f;
        enemy->TakeDamage(turret->_damage);

        auto particleGenerators = turret->GetOwnerNode()->GetAllComponents<ParticleGenerator>();
        for (const auto& generator : particleGenerators) {
            generator->SpawnParticles();
        }
    }
}

bool TurretsManager::isPointInRectangle(const glm::vec3& M, const std::vector<glm::vec3>& rect) {
    if (rect.size() != 4) {
        std::cerr << "Prostokat musi miec dokladnie 4 wierzcholki, a ten ma: " << rect.size() << std::endl;
        return false;
    }

    glm::vec3 A = rect[0];
    glm::vec3 B = rect[1];
    glm::vec3 D = rect[3];

    // Wektory
    glm::vec3 AB = B - A;
    glm::vec3 AD = D - A;
    glm::vec3 AM = M - A;

    // Iloczyny skalarne
    float AB_AB = glm::dot(AB, AB);
    float AM_AB = glm::dot(AM, AB);
    float AD_AD = glm::dot(AD, AD);
    float AM_AD = glm::dot(AM, AD);

    // Warunki
    return (0 < AM_AB && AM_AB < AB_AB) && (0 < AM_AD && AM_AD < AD_AD);
}

void TurretsManager::MoveTurret(){
    if(_isPlayerInMovingMode){
        //std::cout << "ruszymy dzialko o indeksie" << _indexOfMovingTurret << std::endl;
        //std::cout << NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition().x << "  " << NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition().y << "  " << NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition().z << "  " << std::endl;
        _turrets[_indexOfMovingTurret]->_finalPosition = glm::vec3(2137, 2137, 2137);
        _turrets[_indexOfMovingTurret]->_ownerNode->GetTransform()->SetPosition(NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition());
        _turrets[_indexOfMovingTurret]->_ownerNode->GetTransform()->SetRotation(NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetRotation());
    }
}

void TurretsManager::PlaceMovingTurret(){
    if(IsTooCloseToTurret(NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition())) return;

    _isPlayerInMovingMode = false;
    _turrets[_indexOfMovingTurret]->_isMoving = false;
    CalculateRangePositions(_turrets[_indexOfMovingTurret]);
    _turrets[_indexOfMovingTurret]->_finalPosition = NODESMANAGER.getNodeByName("BlueprintTurret")->GetTransform()->GetPosition();
}

int TurretsManager::RaycastTurrets()
{
    if(_isPlayerInMovingMode) return -1;

    selectedIndex = -1; // Initialize with invalid index

    auto camera = ComponentsManager::getInstance().GetComponentByID<Camera>(2);
    glm::vec3 cameraPosition = camera->GetPosition();
    glm::vec3 rayDirection = camera->GetDirection();

    for (size_t i = 0; i < _turrets.size(); ++i) {
        std::string nameOfTurret = "Turret" + std::to_string(i + 1);

        auto meshRenderer = NODESMANAGER.getNodeByName(nameOfTurret)->GetComponent<MeshRenderer>();
        if (meshRenderer) {
            auto model = meshRenderer->_model;

            // Transform bounding box to world space
            glm::vec3 minBoundingBox = model->GetMinBoundingBox();
            glm::vec3 maxBoundingBox = model->GetMaxBoundingBox();
            auto transform = NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform();
            glm::mat4 globalCTM = transform->GetGlobalCTM();

            glm::vec3 transformedMin = glm::vec3(globalCTM * glm::vec4(minBoundingBox, 1.0f));
            glm::vec3 transformedMax = glm::vec3(globalCTM * glm::vec4(maxBoundingBox, 1.0f));

            if (RayIntersectsBoundingBox(cameraPosition, rayDirection, transformedMin, transformedMax)) {
                selectedIndex = i;
                break;
            }
        }
    }

    return selectedIndex; // Return the index of the selected turret (-1 if none selected)
}

bool TurretsManager::RayIntersectsBoundingBox(const glm::vec3& rayOrigin, const glm::vec3& rayDirection,
                                              const glm::vec3& minBoundingBox, const glm::vec3& maxBoundingBox) {
    glm::vec3 invDir = 1.0f / rayDirection;
    glm::vec3 tMin = (minBoundingBox - rayOrigin) * invDir;
    glm::vec3 tMax = (maxBoundingBox - rayOrigin) * invDir;

    glm::vec3 tMinSorted = glm::min(tMin, tMax);
    glm::vec3 tMaxSorted = glm::max(tMin, tMax);

    float tEnter = glm::max(glm::max(tMinSorted.x, tMinSorted.y), tMinSorted.z);
    float tExit = glm::min(glm::min(tMaxSorted.x, tMaxSorted.y), tMaxSorted.z);

    // Check for non-intersection or intersection behind ray origin
    if (tExit < 0 || tEnter > tExit)
        return false;

    // Check for intersection
    return true;
}

bool TurretsManager::isSelectedTurretInRange()
{
    if (selectedIndex != -1)
    {
        std::string nameOfTurret = "Turret" + std::to_string(selectedIndex + 1);

        auto turretPosition = NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->GetPosition();
        auto playerPosition = NODESMANAGER.getNodeByName("player")->GetTransform()->GetPosition();

        float distanceX = (turretPosition.x - playerPosition.x) * (turretPosition.x - playerPosition.x);
        float distanceY = (turretPosition.y - playerPosition.y) * (turretPosition.y - playerPosition.y);
        float distanceZ = (turretPosition.z - playerPosition.z) * (turretPosition.z - playerPosition.z);
        float distance = distanceX + distanceY + distanceZ;

        return distance + 0.01 <= 25;
    }
    return false;
}