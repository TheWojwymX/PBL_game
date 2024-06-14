
//
// Created by Jacek on 13.05.2024.
//

#include "TurretsManager.h"
#include "HUD/PageManager.h"

TurretsManager &TurretsManager::getInstance() {
    static TurretsManager instance;
    return instance;
}

void TurretsManager::Init() {
    for (int i = 0; i < COMPONENTSMANAGER.GetComponents().size(); i++) {
        if (COMPONENTSMANAGER.GetComponents()[i] != nullptr && COMPONENTSMANAGER.GetComponents()[i]->_type == ComponentType::TURRET) {
            _turrets.push_back(std::dynamic_pointer_cast<Turret>(COMPONENTSMANAGER.GetComponents()[i]));
        }
    }
    PrepareBlueprintTurret();

    _PDAController = NODESMANAGER.getNodeByName("PDA")->GetComponent<PDAController>();
}

void TurretsManager::ShowBlueprintTurret(){
    _shouldEnableBlueprintTurret = true;
    _blueprintTurret->GetComponent<MeshRenderer>()->SetEnabled(true);
    NODESMANAGER.getNodeByName("BlueprintRange")->GetComponent<MeshRenderer>()->SetEnabled(true);
    _isInBlueprintMode = true;
    _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;
}

void TurretsManager::HideBlueprintTurret(){
    _shouldEnableBlueprintTurret = false;
    _blueprintTurret->GetComponent<MeshRenderer>()->SetEnabled(false);
    NODESMANAGER.getNodeByName("BlueprintRange")->GetComponent<MeshRenderer>()->SetEnabled(false);
    _isInBlueprintMode = false;
    _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = false;
}

void TurretsManager::ChangeToSpawningMode(){
    _PDAController->HideImmediately();
    HUD._shouldShowCrosshair = true;
    PAGEMANAGER._PDAPage->HidePDAPage();
    _isInTurretChoiceMenu = false;
}

void TurretsManager::PlayerActions(){

    if (INPUT.IsKeyPressed(GLFW_KEY_4) && _PDAController->_isHidden && !_PDAController->_playHideAnim
        && _player->GetTransform()->GetPosition().y >= GAMEMANAGER._groundLevel && !_isPlayerInMovingMode) {
        _PDAController->_playShowAnim = true;
        HUD._shouldShowCrosshair = false;
        PAGEMANAGER._PDAPage->DisplayPDAPage();
        _isInTurretChoiceMenu = true;
        HideBlueprintTurret();
    }
    else if (INPUT.IsKeyPressed(GLFW_KEY_4) && !_PDAController->_isHidden &&
        _player->GetTransform()->GetPosition().y >= GAMEMANAGER._groundLevel && !_isPlayerInMovingMode) {
        _PDAController->HideImmediately();
        HUD._shouldShowCrosshair = true;
        PAGEMANAGER._PDAPage->HidePDAPage();
        _isInTurretChoiceMenu = false;
    }

    if(Input::Instance().IsKeyPressed(GLFW_KEY_1) && _isInTurretChoiceMenu)
    {
        if (GAMEMANAGER._metal < _minigunCost){
            return;
        }
        finaltype = MINIGUN;
        _actualTurretCost = _minigunCost;
        ChangeToSpawningMode();
        ShowBlueprintTurret();
    }
    else if(Input::Instance().IsKeyPressed(GLFW_KEY_2) && _isInTurretChoiceMenu)
    {
        if (GAMEMANAGER._metal < _rifleCost){
            return;
        }
        finaltype = SNIPER;
        _actualTurretCost = _rifleCost;
        ChangeToSpawningMode();
        ShowBlueprintTurret();
    }
    else if(Input::Instance().IsKeyPressed(GLFW_KEY_3) && _isInTurretChoiceMenu)
    {
        if (GAMEMANAGER._metal < _sniperCost){
            return;
        }
        finaltype = RIFLE;
        _actualTurretCost = _sniperCost;
        ChangeToSpawningMode();
        ShowBlueprintTurret();
    }
    else if (INPUT.IsMousePressed(0) && _isInBlueprintMode && !_isPlayerInMovingMode && !IsInForbiddenArea() && !_isInTurretChoiceMenu)
    {
        if (GAMEMANAGER._metal < _actualTurretCost){
            return;
        }
        SpawnTurret(finaltype);
        HideBlueprintTurret();
    }
    else if(INPUT.IsKeyPressed(GLFW_KEY_Q) && (_isInBlueprintMode || _isInTurretChoiceMenu) && !_isPlayerInMovingMode){
        HideBlueprintTurret();
        _PDAController->HideImmediately();
        HUD._shouldShowCrosshair = true;
        PAGEMANAGER._PDAPage->HidePDAPage();
        _isInTurretChoiceMenu = false;
    }

    if (INPUT.IsMousePressed(1) && !_isInBlueprintMode && !_isPlayerInMovingMode && RaycastTurrets() >= 0 && !_turrets[RaycastTurrets()]->_isFlying
        && !_player->GetComponent<PlayerController>()->CheckIfPlayerIsAtEntranceToMine() && !_isInTurretChoiceMenu && IsSelectedTurretInRange()) {

        _indexOfMovingTurret = RaycastTurrets();
        _isPlayerInMovingMode = true;
        _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;
        _turrets[_indexOfMovingTurret]->_isMoving = true;
        _turrets[_indexOfMovingTurret]->_ownerNode->GetParent()->MoveChildToEnd( _turrets[_indexOfMovingTurret]->_ownerNode);
        finaltype = _turrets[_indexOfMovingTurret]->_turretType;
        ShowBlueprintTurret();
    } else if (INPUT.IsMousePressed(0) && _isPlayerInMovingMode && !IsInForbiddenArea() && !_isInTurretChoiceMenu) {
        PlaceMovingTurret();
        HideBlueprintTurret();
    }
}

void TurretsManager::Update() {

    PlayerActions();

    if(IsInForbiddenArea()){
        _additionalColor = glm::vec3(1.0, 0.0, 0.0);

    }else{
        _additionalColor = glm::vec3(0.3647, 0.8353, 0.3647);
    }

    UpdateBlueprintTurret();
    MoveTurret();
    CheckEnemiesInRange();
}

bool TurretsManager::IsInForbiddenArea(){
    if(glm::distance(glm::vec2(_blueprintTurret->GetTransform()->GetPosition().x,
                               _blueprintTurret->GetTransform()->GetPosition().z), GAMEMANAGER._domePosition) > GAMEMANAGER._domeRadius - 0.1
       || glm::distance(glm::vec2(_blueprintTurret->GetTransform()->GetPosition().x,
                                  _blueprintTurret->GetTransform()->GetPosition().z), GAMEMANAGER._domePosition) < GAMEMANAGER._mineEntranceRadius
       || IsTooCloseToTurret(_blueprintTurret->GetTransform()->GetPosition())){
        return true;

    }
    else{
        return false;
    }
}

bool TurretsManager::IsTooCloseToTurret(glm::vec3 pos) {
    for (int i = 0; i < _turrets.size(); i++) {

        if (_turrets[i] == nullptr) continue;

        if (glm::distance(pos, _turrets[i]->_finalPosition) < _distanceToAnotherTurret) {
            return true;
        }
    }
    return false;
}

void TurretsManager::SpawnTurret(turretType type) {

    if (GAMEMANAGER._metal < _actualTurretCost) {
        std::cout << "Brak pieniedzy na dzialko" << std::endl;
        return;
    } else {
        GAMEMANAGER._metal -= _actualTurretCost;
    }

    //Flare spawning
    std::string nameOfFlare = "Flare" + to_string(_turrets.size() + 1);
    shared_ptr<Node> flare = NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), nameOfFlare);

    auto newFlareMeshRenderer = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    newFlareMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("flareModel");
    newFlareMeshRenderer->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
    newFlareMeshRenderer->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    newFlareMeshRenderer->Initiate();
    flare->AddComponent(newFlareMeshRenderer);


    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 360.0f);

    flare->GetTransform()->SetPosition(_blueprintTurret->GetTransform()->GetPosition());
    flare->GetTransform()->SetScale(glm::vec3(0.1, 0.1, 0.1));
    flare->GetTransform()->SetRotation(glm::vec3(flare->GetTransform()->GetRotation().x + 90, flare->GetTransform()->GetRotation().y + dis(gen),
                                                 flare->GetTransform()->GetRotation().z));

    auto flareParticles1 = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"), "flareParticles1");
    flareParticles1->SetOffset(glm::vec3(0.0f, 0.0f, 0.0f));
    flareParticles1->object = flare;
    flareParticles1->Init();
    flare->AddComponent(flareParticles1);

    //Turret spawning
    //std::cout << "zespawniono" << std::endl;
    std::string nameOfTurret = "Turret" + to_string(_turrets.size() + 1);
    std::string rangeIndicatorNode = "Range" + to_string(_turrets.size() + 1);
    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), nameOfTurret);

    auto newMeshRenderer = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    newMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("Turret_Parachute");
    newMeshRenderer->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
    newMeshRenderer->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    newMeshRenderer->Initiate();
    NODESMANAGER.getNodeByName(nameOfTurret)->AddComponent(newMeshRenderer);

    auto bulletParticles = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"), "turretShot");
    bulletParticles->SetOffset(glm::vec3(0.0f, 1.0f, 0.0f));
    bulletParticles->object = NODESMANAGER.getNodeByName(nameOfTurret);
    bulletParticles->Init();
    NODESMANAGER.getNodeByName(nameOfTurret)->AddComponent(bulletParticles);

    auto casingParticles = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"), "turretCasing");
    casingParticles->SetOffset(glm::vec3(0.0f, 1.0f, 0.0f));
    casingParticles->object = NODESMANAGER.getNodeByName(nameOfTurret);
    casingParticles->Init();
    NODESMANAGER.getNodeByName(nameOfTurret)->AddComponent(casingParticles);

    auto newTurret = COMPONENTSMANAGER.CreateComponent<Turret>();
    newTurret->Initiate();
    newTurret->_isFlying = true;
    newTurret->_turretType = type;
    newTurret->setUp();
    newTurret->_finalRotation = _blueprintTurret->GetTransform()->GetRotation();
    newTurret->_finalPosition = _blueprintTurret->GetTransform()->GetPosition();
    newTurret->_flare = NODESMANAGER.getNodeByName(nameOfFlare);

    NODESMANAGER.getNodeByName(nameOfTurret)->AddComponent(newTurret);

    NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->SetPosition(glm::vec3(_blueprintTurret->GetTransform()->GetPosition().x,
                                                                                    _blueprintTurret->GetTransform()->GetPosition().y + 10,
                                                                                    _blueprintTurret->GetTransform()->GetPosition().z));
    NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->SetScale(_blueprintTurret->GetTransform()->GetScale());
    NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->SetRotation(_blueprintTurret->GetTransform()->GetRotation());

    NODESMANAGER.createNode(NODESMANAGER.getNodeByName(nameOfTurret), rangeIndicatorNode);

    auto rangeNode = NODESMANAGER.getNodeByName(rangeIndicatorNode);
    auto rangeIndicator = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    rangeIndicator->_model = RESOURCEMANAGER.GetModelByName("sandModel");
    rangeIndicator->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
    rangeIndicator->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    rangeIndicator->Initiate();
    rangeIndicator->SetEnabled(false);
    rangeNode->AddComponent(rangeIndicator);
    rangeNode->GetTransform()->AddPosition(glm::vec3(0.0f, 0.0f, 30.0f));
    rangeNode->GetTransform()->SetScale(glm::vec3(_sideRange, 0.2f, _forwardRange));

    NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->UpdateGlobalCTM();
    rangeNode->GetTransform()->UpdateGlobalCTM();

    CalculateRangePositions(newTurret);

    _turrets.push_back(NODESMANAGER.getNodeByName(nameOfTurret)->GetComponent<Turret>());
}

void TurretsManager::CalculateRangePositions(shared_ptr<Turret> turret) {
    std::vector<glm::vec3> corners;
    auto rangeNodes = turret->_ownerNode->getChildren();
    for (const auto &node: rangeNodes) {
        if (node != nullptr) {
            corners = FrustumCulling::GetRange(node->GetComponent<MeshRenderer>()->_model,
                                               node->GetTransform()->GetGlobalCTM());
        }
    }
    if (!corners.empty()) {
        turret->_turretRangePositions[0] = corners[0];
        turret->_turretRangePositions[1] = corners[1];
        turret->_turretRangePositions[2] = corners[3];
        turret->_turretRangePositions[3] = corners[5];
    }
}

void TurretsManager::PrepareBlueprintTurret() {
    std::string nameOfBlueprintTurret = "BlueprintTurret";
    std::string blueprintRange = "BlueprintRange";

    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), nameOfBlueprintTurret);

    auto newMeshRenderer = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    newMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("Turret_Minigun_Level1");
    newMeshRenderer->_shader = RESOURCEMANAGER.GetShaderByName("blueprintShader");
    newMeshRenderer->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    newMeshRenderer->Initiate();

    _blueprintTurret = NODESMANAGER.getNodeByName(nameOfBlueprintTurret);
    _blueprintTurret->AddComponent(newMeshRenderer);
    _blueprintTurret->GetComponent<MeshRenderer>()->SetEnabled(false);

    NODESMANAGER.createNode(_blueprintTurret, blueprintRange);
    auto rangeIndicator = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    rangeIndicator->_model = RESOURCEMANAGER.GetModelByName("sandModel");
    rangeIndicator->_shader = RESOURCEMANAGER.GetShaderByName("blueprintShader");
    rangeIndicator->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    rangeIndicator->SetEnabled(false);
    rangeIndicator->Initiate();

    NODESMANAGER.getNodeByName(blueprintRange)->AddComponent(rangeIndicator);
    NODESMANAGER.getNodeByName(blueprintRange)->GetTransform()->AddPosition(glm::vec3(0.0f, 0.0f, 30.0f));
    NODESMANAGER.getNodeByName(blueprintRange)->GetTransform()->SetScale(glm::vec3(_sideRange, 0.2f, _forwardRange));
}


void TurretsManager::UpdateBlueprintTurret() {

    if(!_shouldEnableBlueprintTurret) return;

    std::string nameOfBlueprintTurret = "BlueprintTurret";
    switch(finaltype)
    {
        case MINIGUN:
            NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetComponent<MeshRenderer>()->_model = RESOURCEMANAGER.GetModelByName("Turret_Minigun_Level1");
            break;

        case SNIPER:
            NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetComponent<MeshRenderer>()->_model = RESOURCEMANAGER.GetModelByName("Turret_Sniper_Level1");
            break;

        case RIFLE:
            NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetComponent<MeshRenderer>()->_model = RESOURCEMANAGER.GetModelByName("Turret_Rifle_Level1");
            break;
    }

    glm::vec3 forwardVector = glm::normalize(_player->GetComponent<Camera>()->GetFrontVector());
    float minDistance = 2.0f;
    glm::vec3 turretPosition = _player->GetTransform()->GetPosition() + forwardVector * minDistance;
    turretPosition.y = GAMEMANAGER._groundLevel;
    glm::vec2 playerFlatPosition(_player->GetTransform()->GetPosition().x, _player->GetTransform()->GetPosition().z);
    glm::vec2 turretFlatPosition(turretPosition.x, turretPosition.z);
    if (glm::distance(playerFlatPosition, turretFlatPosition) < minDistance) {
        glm::vec2 direction = glm::normalize(turretFlatPosition - playerFlatPosition);
        turretFlatPosition = playerFlatPosition + direction * minDistance;
        turretPosition.x = turretFlatPosition.x;
        turretPosition.z = turretFlatPosition.y;
    }
    _blueprintTurret->GetTransform()->SetPosition(turretPosition);

    _blueprintTurret->GetTransform()->SetScale(0.3);

    glm::vec3 lookAtPoint = turretPosition + forwardVector;
    lookAtPoint.y = turretPosition.y;
    glm::vec3 lookDirection = glm::normalize(lookAtPoint - turretPosition);
    _blueprintTurret->GetTransform()->LookAt(lookDirection);

    //positions counting
    glm::vec3 BPPosition = _blueprintTurret->GetTransform()->GetPosition();
    glm::vec3 upVec = glm::vec3(0, 1, 0);
    glm::vec3 forwardVec = _blueprintTurret->GetTransform()->GetRotation() * glm::vec3(0, 0, 1);
    glm::vec3 backwardVec = -forwardVec;
    glm::vec3 leftVec = glm::normalize(glm::cross(upVec, forwardVec));
    glm::vec3 rightVec = -leftVec;

    glm::vec3 pos0 = BPPosition + leftVec * _sideRange + backwardVec * _backRange;
    glm::vec3 pos1 = BPPosition + forwardVec * _forwardRange + leftVec * _sideRange;
    glm::vec3 pos2 = BPPosition + forwardVec * _forwardRange + rightVec * _sideRange;
    glm::vec3 pos3 = BPPosition + rightVec * _sideRange + backwardVec * _backRange;

    _blueprintTurret->GetParent()->MoveChildToEnd(_blueprintTurret);
}

void TurretsManager::CheckEnemiesInRange() {
    for (int i = 0; i < _turrets.size(); i++) {

        if (_turrets[i] == nullptr) continue;

        if (_turrets[i]->_isFlying) continue;

        if (_turrets[i]->_isMoving) continue;

        std::vector<shared_ptr<Enemy>> enemiesInRange;

        for (int j = 0; j < ENEMIESMANAGER._enemies.size(); j++) {

            if (ENEMIESMANAGER._enemies[j] == nullptr) continue;

            if (ENEMIESMANAGER._enemies[j]->_hp <= 0) continue;

            glm::vec3 enemyPos = ENEMIESMANAGER._enemies[j]->GetOwnerPosition();

            if (isPointInRectangle(enemyPos, _turrets[i]->_turretRangePositions)) {
                enemiesInRange.push_back(ENEMIESMANAGER._enemies[j]);
            }
        }

        std::shared_ptr<Enemy> targetEnemy = nullptr;
        float closestDistance = std::numeric_limits<float>::max();

        for (const auto& enemy : enemiesInRange) {
            float distance = glm::distance(_turrets[i]->GetOwnerPosition(), enemy->GetOwnerPosition());

            switch (_turrets[i]->_turretType) {
                case MINIGUN:
                    if (enemy->_enemyType == ANT) {
                        if (distance < closestDistance) {
                            targetEnemy = enemy;
                            closestDistance = distance;
                        }
                    } else if (enemy->_enemyType == BEETLE && targetEnemy == nullptr) {
                        targetEnemy = enemy;
                    }
                    break;

                case SNIPER:
                    if (enemy->_enemyType == BEETLE) {
                        if (distance < closestDistance) {
                            targetEnemy = enemy;
                            closestDistance = distance;
                        }
                    } else if (enemy->_enemyType == ANT && targetEnemy == nullptr) {
                        targetEnemy = enemy;
                    }
                    break;

                case RIFLE:
                    if (enemy->_enemyType == WASP) {
                        if (distance < closestDistance) {
                            targetEnemy = enemy;
                            closestDistance = distance;
                        }
                    }
                    break;
            }
        }

        if (targetEnemy) {
            glm::vec3 turretPosition = _turrets[i]->_ownerNode->GetTransform()->GetPosition();
            glm::vec3 enemyPosition = targetEnemy->GetOwnerPosition();
            enemyPosition.y = turretPosition.y;
            glm::vec3 lookDirection = glm::normalize(enemyPosition - turretPosition);
            _turrets[i]->_ownerNode->GetTransform()->LookAt(lookDirection);

            AttackEnemy(_turrets[i], targetEnemy);
        } else {
            Reload(_turrets[i]);
        }
    }
}

void TurretsManager::Reload(const shared_ptr<Turret> &turret) {
    if (turret->_timer < turret->_fireRate) {
        turret->_timer += TIME.GetDeltaTime();
    }
}

void TurretsManager::AttackEnemy(const shared_ptr<Turret> &turret, const shared_ptr<Enemy> &enemy) {
    if (turret->_timer < turret->_fireRate)
    {
        turret->_timer += TIME.GetDeltaTime();
    }
    else
    {
        turret->_timer = 0.0f;

        int finalDamage;
        turretType _turretType = turret->_turretType;
        enemyType _enemyType = enemy->_enemyType;

        switch(turret->_turretType)
        {
            case MINIGUN:
                if (_enemyType == BEETLE)
                {
                    finalDamage = 0.5 * turret->_damage;
                }
                else
                {
                    finalDamage = turret->_damage;
                }
                break;

            case SNIPER:
                if (_enemyType == ANT)
                {
                    finalDamage = 0.5 * turret->_damage;
                }
                else
                {
                    finalDamage = turret->_damage;
                }
                break;

            case RIFLE:
                finalDamage = turret->_damage;
                break;
        }

        enemy->TakeDamage(finalDamage);

        auto particleGenerators = turret->GetOwnerNode()->GetAllComponents<ParticleGenerator>();
        for (const auto &generator: particleGenerators) {
            if (generator != nullptr) {
                generator->enemyPosition = enemy->GetOwnerPosition();
                generator->SpawnParticles();

                static std::random_device rd;
                static std::mt19937 gen(rd());
                std::uniform_int_distribution<> dis(12, 13);

                RESOURCEMANAGER.GetSoundByID(dis(gen))->PlaySoundSim(turret->_ownerNode);
            }
        }
    }
}

bool TurretsManager::isPointInRectangle(const glm::vec3 &M, const std::vector<glm::vec3> &rect) {
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

void TurretsManager::MoveTurret() {
    if (_isPlayerInMovingMode) {
        //std::cout << "ruszymy dzialko o indeksie" << _indexOfMovingTurret << std::endl;
        //std::cout << _blueprintTurret->GetTransform()->GetPosition().x << "  " << _blueprintTurret->GetTransform()->GetPosition().y << "  " << _blueprintTurret->GetTransform()->GetPosition().z << "  " << std::endl;
        _turrets[_indexOfMovingTurret]->_finalPosition = glm::vec3(2137, 2137, 2137);
        _turrets[_indexOfMovingTurret]->_ownerNode->GetTransform()->SetPosition(_blueprintTurret->GetTransform()->GetPosition());
        _turrets[_indexOfMovingTurret]->_ownerNode->GetTransform()->SetRotation(_blueprintTurret->GetTransform()->GetRotation());
        _turrets[_indexOfMovingTurret]->_ownerNode->GetComponent<MeshRenderer>()->_shader = RESOURCEMANAGER.GetShaderByName("blueprintShader");
        _turrets[_indexOfMovingTurret]->_ownerNode->getFirstChild()->GetComponent<MeshRenderer>()->_shader = RESOURCEMANAGER.GetShaderByName("blueprintShader");
    }
}

void TurretsManager::PlaceMovingTurret() {
    _isPlayerInMovingMode = false;
    _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = false;
    _turrets[_indexOfMovingTurret]->_isMoving = false;
    CalculateRangePositions(_turrets[_indexOfMovingTurret]);
    _turrets[_indexOfMovingTurret]->_finalPosition = _blueprintTurret->GetTransform()->GetPosition();

    _turrets[_indexOfMovingTurret]->_ownerNode->GetComponent<MeshRenderer>()->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
    _turrets[_indexOfMovingTurret]->_ownerNode->getFirstChild()->GetComponent<MeshRenderer>()->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
}

int TurretsManager::RaycastTurrets() {
    if (_isPlayerInMovingMode) return -1;

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

bool TurretsManager::RayIntersectsBoundingBox(const glm::vec3 &rayOrigin, const glm::vec3 &rayDirection,
                                              const glm::vec3 &minBoundingBox, const glm::vec3 &maxBoundingBox) {
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

bool TurretsManager::IsSelectedTurretInRange() {
    if (selectedIndex != -1) {
        std::string nameOfTurret = "Turret" + std::to_string(selectedIndex + 1);

        auto turretPosition = NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->GetPosition();
        auto playerPosition = _player->GetTransform()->GetPosition();

        float distanceX = (turretPosition.x - playerPosition.x) * (turretPosition.x - playerPosition.x);
        float distanceY = (turretPosition.y - playerPosition.y) * (turretPosition.y - playerPosition.y);
        float distanceZ = (turretPosition.z - playerPosition.z) * (turretPosition.z - playerPosition.z);
        float distance = distanceX + distanceY + distanceZ;

        return distance + 0.01 <= 12;
    }
    return false;
}