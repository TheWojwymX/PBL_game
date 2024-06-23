#include "TurretsManager.h"
#include "HUD/PageManager.h"
#include "Managers/UpgradeManager.h"

TurretsManager &TurretsManager::getInstance() {
    static TurretsManager instance;
    return instance;
}

void TurretsManager::Init() {
    _turretCosts = {
            1, // Minigun
            1, // Sniper
            1  // Rifle
    };
    InitTurretStats();

    for (int i = 0; i < COMPONENTSMANAGER.GetComponents().size(); i++) {
        if (COMPONENTSMANAGER.GetComponents()[i] != nullptr && COMPONENTSMANAGER.GetComponents()[i]->_type == ComponentType::TURRET) {
            _turrets.push_back(std::dynamic_pointer_cast<Turret>(COMPONENTSMANAGER.GetComponents()[i]));
        }
    }
    PrepareBlueprintTurret();

    _PDAController = NODESMANAGER.getNodeByName("PDA")->GetComponent<PDAController>();
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(2);
}

void TurretsManager::ShowBlueprintTurret() {
    _shouldEnableBlueprintTurret = true;
    auto blueprintRange = NODESMANAGER.getNodeByName("BlueprintRange");
    if (_heldTurret != nullptr) {
        blueprintRange->GetTransform()->SetScale(glm::vec3(_heldTurret->GetSideRange(), 0.2f, _heldTurret->GetForwardRange()));
    }
    else {
        _blueprintTurret->GetComponent<MeshRenderer>()->SetEnabled(true);
        blueprintRange->GetTransform()->SetScale(glm::vec3(_turretStats[_turretType].z, 0.2f, _turretStats[_turretType].w));
    } 
    blueprintRange->GetComponent<MeshRenderer>()->SetEnabled(true);
    _isInBlueprintMode = true;
    _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;
}

void TurretsManager::HideBlueprintTurret() {
    _shouldEnableBlueprintTurret = false;
    _blueprintTurret->GetComponent<MeshRenderer>()->SetEnabled(false);
    NODESMANAGER.getNodeByName("BlueprintRange")->GetComponent<MeshRenderer>()->SetEnabled(false);
    _isInBlueprintMode = false;
    _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = false;
    _heldTurret = nullptr;
}

void TurretsManager::ChangeToSpawningMode() {
    _PDAController->HideImmediately();
    HUD._shouldShowCrosshair = true;
    PAGEMANAGER._PDAPage->HidePDAPage();
    _isInTurretChoiceMenu = false;
}

void TurretsManager::PlayerActions() {

    auto playerPosY = _player->GetTransform()->GetPosition().y;

    if (_player->GetComponent<PlayerController>()->CheckIfPlayerIsAtEntranceToMine() && _isInTurretChoiceMenu) {
        _PDAController->HideImmediately();
        _isInTurretChoiceMenu = false;
        PAGEMANAGER._PDAPage->HidePDAPage();
    }

    if (INPUT.IsKeyPressed(GLFW_KEY_4) && _PDAController->_isHidden && !_PDAController->_playHideAnim &&
        !_player->GetComponent<PlayerController>()->CheckIfPlayerIsAtEntranceToMine() && playerPosY >= GAMEMANAGER._groundLevel &&
        !_isPlayerInMovingMode && HUD._isAfterTutorialCrosshair && !PAGEMANAGER._isInUpgradeMenu) {
        _PDAController->_playShowAnim = true;
        HUD._shouldShowCrosshair = false;
        PAGEMANAGER._PDAPage->DisplayPDAPage();
        _isInTurretChoiceMenu = true;
        HideBlueprintTurret();
    } else if ((INPUT.IsKeyPressed(GLFW_KEY_4) || INPUT.IsMousePressed(1) || INPUT.IsKeyPressed(GLFW_KEY_Q)) && !_PDAController->_isHidden &&
               playerPosY >= GAMEMANAGER._groundLevel && !_isPlayerInMovingMode) {
        _PDAController->HideImmediately();
        HUD._shouldShowCrosshair = true;
        PAGEMANAGER._PDAPage->HidePDAPage();
        _isInTurretChoiceMenu = false;
    }

    if (Input::Instance().IsKeyPressed(GLFW_KEY_1) && _isInTurretChoiceMenu) {
        if (!GAMEMANAGER.HasPlastic(_turretCosts[TurretType::MINIGUN])) {
            return;
        }
        _turretType = MINIGUN;
        ChangeToSpawningMode();
        ShowBlueprintTurret();
    } else if (Input::Instance().IsKeyPressed(GLFW_KEY_2) && _isInTurretChoiceMenu) {
        if (!GAMEMANAGER.HasPlastic(_turretCosts[TurretType::SNIPER])) {
            return;
        }
        _turretType = SNIPER;
        ChangeToSpawningMode();
        ShowBlueprintTurret();
    } else if (Input::Instance().IsKeyPressed(GLFW_KEY_3) && _isInTurretChoiceMenu) {
        if (!GAMEMANAGER.HasPlastic(_turretCosts[TurretType::RIFLE])) {
            return;
        }
        _turretType = RIFLE;
        ChangeToSpawningMode();
        ShowBlueprintTurret();
    } else if (INPUT.IsMousePressed(0) && _isInBlueprintMode && !_isPlayerInMovingMode && !IsInForbiddenArea() && !_isInTurretChoiceMenu) {
        if (!GAMEMANAGER.HasPlastic(_turretCosts[_turretType])) {
            return;
        }
        SpawnTurret(_turretType);
        HideBlueprintTurret();
    } else if ((INPUT.IsKeyPressed(GLFW_KEY_Q) || INPUT.IsMousePressed(1)) && (_isInBlueprintMode || _isInTurretChoiceMenu) && !_isPlayerInMovingMode) {
        HideBlueprintTurret();
        _PDAController->_playShowAnim = true;
        HUD._shouldShowCrosshair = false;
        PAGEMANAGER._PDAPage->DisplayPDAPage();
        _isInTurretChoiceMenu = true;
    }

    if (INPUT.IsMousePressed(1) && !_isInBlueprintMode && !_isPlayerInMovingMode && _selectedIndex >= 0 && !_turrets[_selectedIndex]->_isFlying
        && !_player->GetComponent<PlayerController>()->CheckIfPlayerIsAtEntranceToMine() && !_isInTurretChoiceMenu && IsSelectedTurretInRange()) {

        _indexOfMovingTurret = _selectedIndex;
        _isPlayerInMovingMode = true;
        _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;
        _turrets[_indexOfMovingTurret]->_isMoving = true;
        _heldTurret = _turrets[_indexOfMovingTurret];
        ShowBlueprintTurret();
        RESOURCEMANAGER.GetSoundByName("PickUp")->PlaySound(_player);
    } else if (INPUT.IsMousePressed(0) && _isPlayerInMovingMode && !IsInForbiddenArea() && !_isInTurretChoiceMenu) {
        PlaceMovingTurret();
        HideBlueprintTurret();
    }
}

void TurretsManager::Update() {
    RaycastTurrets();
    PlayerActions();

    if (IsInForbiddenArea()) {
        _additionalColor = glm::vec3(1.0, 0.0, 0.0);

    } else {
        _additionalColor = glm::vec3(0.3647, 0.8353, 0.3647);
    }

    UpdateBlueprintTurret();
    MoveTurret();
    CheckEnemiesInRange();
}

bool TurretsManager::IsInForbiddenArea() {
    if (glm::distance(glm::vec2(_blueprintTurret->GetTransform()->GetPosition().x,
                                _blueprintTurret->GetTransform()->GetPosition().z), GAMEMANAGER._domePosition) > GAMEMANAGER._domeRadius - 0.1

        || glm::distance(glm::vec2(_blueprintTurret->GetTransform()->GetPosition().x,
                                   _blueprintTurret->GetTransform()->GetPosition().z), GAMEMANAGER._playerStationPosition) <
           GAMEMANAGER._playerStationRadius

        || glm::distance(glm::vec2(_blueprintTurret->GetTransform()->GetPosition().x,
                                   _blueprintTurret->GetTransform()->GetPosition().z), GAMEMANAGER._domeStationPosition) <
           GAMEMANAGER._domeStationRadius

        || glm::distance(glm::vec2(_blueprintTurret->GetTransform()->GetPosition().x,
                                   _blueprintTurret->GetTransform()->GetPosition().z), GAMEMANAGER._domePosition) < GAMEMANAGER._mineEntranceRadius
                                   
        || IsTooCloseToTurret(_blueprintTurret->GetTransform()->GetPosition())) {
        return true;

    } else {
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

void TurretsManager::SpawnTurret(TurretType type) {

    GAMEMANAGER.RemovePlastic(_turretCosts[type]);
    _turretCosts[type] += _turretCostIncrease;

    //Flare spawning
    std::string nameOfFlare = "Flare" + to_string(_newTurretIndex);
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

    auto posX = _blueprintTurret->GetTransform()->GetPosition().x;
    auto posY = _blueprintTurret->GetTransform()->GetPosition().y + 0.2;
    auto posZ = _blueprintTurret->GetTransform()->GetPosition().z;
    flare->GetTransform()->SetPosition(glm::vec3(posX, posY, posZ));
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
    std::string nameOfTurret = "Turret" + to_string(_newTurretIndex);
    std::string rangeIndicatorNode = "Range" + to_string(_newTurretIndex);
    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), nameOfTurret);

    auto newTurret = COMPONENTSMANAGER.CreateComponent<Turret>();
    newTurret->Initiate();
    newTurret->_isFlying = true;
    newTurret->SetTurretType(type);
    newTurret->SetUp(_turretStats[_turretType]);
    newTurret->_finalRotation = _blueprintTurret->GetTransform()->GetRotation();
    newTurret->_finalPosition = _blueprintTurret->GetTransform()->GetPosition();
    newTurret->_flare = NODESMANAGER.getNodeByName(nameOfFlare);
    NODESMANAGER.getNodeByName(nameOfTurret)->AddComponent(newTurret);

    auto newMeshRenderer = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    if(_turretType == MINIGUN) newMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("Minigun_Parachute");
    else if(_turretType == SNIPER) newMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("Sniper_Parachute");
    else if(_turretType == RIFLE) newMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("Rifle_Parachute");

    newMeshRenderer->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
    newMeshRenderer->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    newMeshRenderer->_transparent = true;
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

    const char *texture;
    if(_turretType == MINIGUN) texture = "res/Particle/Tooltips/minigun_lvl1.png";
    else if(_turretType == SNIPER) texture = "res/Particle/Tooltips/sniper_lvl1.png";
    else if(_turretType == RIFLE) texture = "res/Particle/Tooltips/rifle_lvl1.png";

    auto tooltipParticle = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"), "tooltipParticle");
    tooltipParticle->SetOffset(glm::vec3(0.0f, 0.8f, 0.0f));
    tooltipParticle->object = NODESMANAGER.getNodeByName(nameOfTurret);
    tooltipParticle->Init();
    tooltipParticle->texture = Texture2D::loadTextureFromFile(texture, true);
    NODESMANAGER.getNodeByName(nameOfTurret)->AddComponent(tooltipParticle);

    NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->SetPosition(glm::vec3(_blueprintTurret->GetTransform()->GetPosition().x,
                                                                                    _blueprintTurret->GetTransform()->GetPosition().y + 10,
                                                                                    _blueprintTurret->GetTransform()->GetPosition().z));
    NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->SetScale(_blueprintTurret->GetTransform()->GetScale());
    NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->SetRotation(_blueprintTurret->GetTransform()->GetRotation());

    NODESMANAGER.createNode(NODESMANAGER.getNodeByName(nameOfTurret), rangeIndicatorNode);

    auto rangeNode = NODESMANAGER.getNodeByName(rangeIndicatorNode);
    auto rangeIndicator = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    rangeIndicator->_model = RESOURCEMANAGER.GetModelByName("RangeIndicatorVisual");
    rangeIndicator->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
    rangeIndicator->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    rangeIndicator->_disableShadows = true;
    rangeIndicator->Initiate();
    rangeIndicator->_transparent = true;
    rangeIndicator->SetEnabled(false);
    rangeNode->AddComponent(rangeIndicator);
    rangeNode->GetTransform()->SetScale(glm::vec3(newTurret->GetSideRange(), 0.2f, newTurret->GetForwardRange()));
    rangeNode->GetTransform()->AddPosition(glm::vec3(0.0f, 0.0f, 3.0f));

    NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->UpdateGlobalCTM();
    rangeNode->GetTransform()->UpdateGlobalCTM();

    _turrets.push_back(NODESMANAGER.getNodeByName(nameOfTurret)->GetComponent<Turret>());

    _newTurretIndex++;
}

void TurretsManager::PrepareBlueprintTurret() {
    std::string nameOfBlueprintTurret = "BlueprintTurret";
    std::string blueprintRange = "BlueprintRange";

    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), nameOfBlueprintTurret);

    auto newMeshRenderer = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    newMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("Turret_Minigun_Level1");
    newMeshRenderer->_shader = RESOURCEMANAGER.GetShaderByName("blueprintShader");
    newMeshRenderer->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    newMeshRenderer->_transparent = true;
    newMeshRenderer->_disableShadows = true;
    newMeshRenderer->Initiate();

    _blueprintTurret = NODESMANAGER.getNodeByName(nameOfBlueprintTurret);
    _blueprintTurret->AddComponent(newMeshRenderer);
    _blueprintTurret->GetComponent<MeshRenderer>()->SetEnabled(false);

    NODESMANAGER.createNode(_blueprintTurret, blueprintRange);
    auto rangeIndicator = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    rangeIndicator->_model = RESOURCEMANAGER.GetModelByName("RangeIndicatorVisual");
    rangeIndicator->_shader = RESOURCEMANAGER.GetShaderByName("blueprintShader");
    rangeIndicator->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    rangeIndicator->_disableShadows = true;
    rangeIndicator->_transparent = true;
    rangeIndicator->SetEnabled(false);
    rangeIndicator->Initiate();

    NODESMANAGER.getNodeByName(blueprintRange)->AddComponent(rangeIndicator);
    NODESMANAGER.getNodeByName(blueprintRange)->GetTransform()->SetScale(glm::vec3(50.0f, 0.2f, 50.0f));
    NODESMANAGER.getNodeByName(blueprintRange)->GetTransform()->AddPosition(glm::vec3(0.0f, 0.0f, 3.0f));
}


void TurretsManager::UpdateBlueprintTurret() {

    if (!_shouldEnableBlueprintTurret) return;

    std::string nameOfBlueprintTurret = "BlueprintTurret";
    std::string blueprintRange = "BlueprintRange";
    char turretLevel = 0;

    if (_heldTurret != nullptr)
        turretLevel = _heldTurret->GetUpgradeLevel();
    switch (_turretType) {
        case MINIGUN: {
            switch (turretLevel) {
            case 0:
                NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetComponent<MeshRenderer>()->_model =
                    RESOURCEMANAGER.GetModelByName("Turret_Minigun_Level1");
                break;
            case 1:
                NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetComponent<MeshRenderer>()->_model =
                    RESOURCEMANAGER.GetModelByName("Turret_Minigun_Level2");
                break;
            case 2:
                NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetComponent<MeshRenderer>()->_model =
                    RESOURCEMANAGER.GetModelByName("Turret_Minigun_Level3");
                break;
            default:
                std::cerr << "Unknown level for Minigun turret: " << turretLevel << std::endl;
                break;
            }
            break;
        }
        case SNIPER: {
            switch (turretLevel) {
            case 0:
                NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetComponent<MeshRenderer>()->_model =
                    RESOURCEMANAGER.GetModelByName("Turret_Sniper_Level1");
                break;
            case 1:
                NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetComponent<MeshRenderer>()->_model =
                    RESOURCEMANAGER.GetModelByName("Turret_Sniper_Level2");
                break;
            case 2:
                NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetComponent<MeshRenderer>()->_model =
                    RESOURCEMANAGER.GetModelByName("Turret_Sniper_Level3");
                break;
            default:
                std::cerr << "Unknown level for Sniper turret: " << turretLevel << std::endl;
                break;
            }
            break;
        }
        case RIFLE: {
            switch (turretLevel) {
            case 0:
                NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetComponent<MeshRenderer>()->_model =
                    RESOURCEMANAGER.GetModelByName("Turret_Rifle_Level1");
                break;
            case 1:
                NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetComponent<MeshRenderer>()->_model =
                    RESOURCEMANAGER.GetModelByName("Turret_Rifle_Level2");
                break;
            case 2:
                NODESMANAGER.getNodeByName(nameOfBlueprintTurret)->GetComponent<MeshRenderer>()->_model =
                    RESOURCEMANAGER.GetModelByName("Turret_Rifle_Level3");
                break;
            default:
                std::cerr << "Unknown level for Rifle turret: " << turretLevel << std::endl;
                break;
            }
            break;
        }
        default:
            std::cerr << "Unknown turret type: " << _heldTurret->GetTurretType() << std::endl;
            break;
    }


    glm::vec3 forwardVector = glm::normalize(_player->GetComponent<Camera>()->GetFrontVector());
    glm::vec3 playerPos = _player->GetTransform()->GetPosition();

    float forwardDistance = 2.0f;

    // Calculate turret position
    glm::vec3 turretPosition = playerPos + forwardVector * forwardDistance;  // Move forwardDistance along the forward vector
    turretPosition.y = GAMEMANAGER._groundLevel;

    _blueprintTurret->GetTransform()->SetPosition(turretPosition);
    _blueprintTurret->GetTransform()->SetScale(0.3);

    glm::vec3 lookAtPoint = turretPosition + forwardVector;
    lookAtPoint.y = turretPosition.y;
    glm::vec3 lookDirection = glm::normalize(lookAtPoint - turretPosition);
    _blueprintTurret->GetTransform()->LookAt(lookDirection);
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

            if (IsPointInTrapezoid(enemyPos, _turrets[i]->_turretRangePositions)) {
                enemiesInRange.push_back(ENEMIESMANAGER._enemies[j]);
            }
        }

        std::shared_ptr<Enemy> targetEnemy = nullptr;
        float closestDistance = std::numeric_limits<float>::max();

        for (const auto &enemy: enemiesInRange) {
            float distance = glm::distance(_turrets[i]->GetOwnerPosition(), enemy->GetOwnerPosition());

            switch (_turrets[i]->GetTurretType()) {
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

bool TurretsManager::IsPointInTrapezoid(glm::vec3 point, std::vector<glm::vec3> trapPoints) {
    // Ensure trapPoints has exactly 4 points
    if (trapPoints.size() != 4) {
        throw std::invalid_argument("Trapezoid must have exactly 4 points.");
    }

    // Extract vertices of the trapezoid in 2D (x, z)
    glm::vec2 A(trapPoints[0].x, trapPoints[0].z);
    glm::vec2 B(trapPoints[1].x, trapPoints[1].z);
    glm::vec2 C(trapPoints[2].x, trapPoints[2].z);
    glm::vec2 D(trapPoints[3].x, trapPoints[3].z);

    // Point in 2D (x, z)
    glm::vec2 P(point.x, point.z);

    // Check if point P lies inside the trapezoid using cross product method
    // AB x AP, BC x BP, CD x CP, DA x DP must have the same sign for P to be inside the trapezoid

    auto cross2D = [](glm::vec2 v1, glm::vec2 v2) {
        return v1.x * v2.y - v1.y * v2.x;
    };

    bool inside = (cross2D(B - A, P - A) >= 0) &&
                  (cross2D(C - B, P - B) >= 0) &&
                  (cross2D(D - C, P - C) >= 0) &&
                  (cross2D(A - D, P - D) >= 0);

    return inside;
}

void TurretsManager::InitTurretStats() {
    // Initialize the _turretStats vector with values for MINIGUN, SNIPER, and RIFLE
    // fireRate, dmg, sideRange, forwardRange
    _turretStats.push_back(glm::vec4(0.5f, 1.0f, 80.0f, 60.0f));  // MINIGUN
    _turretStats.push_back(glm::vec4(2.0f, 5.0f, 50.0f, 100.0f)); // SNIPER
    _turretStats.push_back(glm::vec4(1.0f, 1.0f, 80.0f, 80.0f));  // RIFLE

    _turretCostIncrease = 2;
}

void TurretsManager::Reload(const shared_ptr<Turret> &turret) {
    if (turret->_timer < turret->GetFireRate()) {
        turret->_timer += TIME.GetDeltaTime();
    }
}

void TurretsManager::AttackEnemy(const shared_ptr<Turret> &turret, const shared_ptr<Enemy> &enemy) {
    if (turret->_timer < turret->GetFireRate()) {
        turret->_timer += TIME.GetDeltaTime();
    } else {
        turret->_timer = 0.0f;

        float finalDamage = 0.0f;
        TurretType _turretType = turret->GetTurretType();
        EnemyType _enemyType = enemy->_enemyType;

        switch (_turretType) {
            case MINIGUN:
                if (_enemyType == BEETLE) {
                    finalDamage = 0.25 * turret->GetDamage();
                } else {
                    finalDamage = turret->GetDamage();
                }
                break;

            case SNIPER:
                finalDamage = turret->GetDamage();
                break;

            case RIFLE:
                finalDamage = turret->GetDamage();
                break;
        }

        enemy->TakeDamage(finalDamage);

        LIGHTSMANAGER.AddShot(turret->GetOwnerPosition(), enemy->GetOwnerPosition());

        auto particleGenerators = turret->GetOwnerNode()->GetAllComponents<ParticleGenerator>();
        for (const auto &generator: particleGenerators) {
            if (generator != nullptr) {
                generator->enemyPosition = enemy->GetOwnerPosition();
                generator->SpawnParticles();
            }
        }

        std::pair<int, float> sound = turret->GetSound();
        RESOURCEMANAGER.GetSoundByID(sound.first)->PlaySoundSim(turret->_ownerNode, sound.second);
    }
}

void TurretsManager::MoveTurret() {
    if (_isPlayerInMovingMode) {
        //std::cout << "ruszymy dzialko o indeksie" << _indexOfMovingTurret << std::endl;
        //std::cout << _blueprintTurret->GetTransform()->GetPosition().x << "  " << _blueprintTurret->GetTransform()->GetPosition().y << "  " << _blueprintTurret->GetTransform()->GetPosition().z << "  " << std::endl;
        _turrets[_indexOfMovingTurret]->_finalPosition = glm::vec3(2137, 2137, 2137);
        _turrets[_indexOfMovingTurret]->_ownerNode->GetTransform()->SetPosition(_blueprintTurret->GetTransform()->GetPosition());
        _turrets[_indexOfMovingTurret]->_ownerNode->GetTransform()->SetRotation(_blueprintTurret->GetTransform()->GetRotation());
        _turrets[_indexOfMovingTurret]->_ownerNode->GetComponent<MeshRenderer>()->_shader = RESOURCEMANAGER.GetShaderByName("blueprintShader");
        _turrets[_indexOfMovingTurret]->_ownerNode->getFirstChild()->GetComponent<MeshRenderer>()->_shader = RESOURCEMANAGER.GetShaderByName(
                "blueprintShader");
    }
}

void TurretsManager::PlaceMovingTurret() {
    _isPlayerInMovingMode = false;
    _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = false;
    _turrets[_indexOfMovingTurret]->_isMoving = false;
    _heldTurret->CalculateRangePositions();
    _turrets[_indexOfMovingTurret]->_finalPosition = _blueprintTurret->GetTransform()->GetPosition();

    _turrets[_indexOfMovingTurret]->_ownerNode->GetComponent<MeshRenderer>()->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
    _turrets[_indexOfMovingTurret]->_ownerNode->getFirstChild()->GetComponent<MeshRenderer>()->_shader = RESOURCEMANAGER.GetShaderByName(
            "modelShader");

    RESOURCEMANAGER.GetSoundByName("Place")->PlaySound(_player);
}

void TurretsManager::RaycastTurrets() {
    if (_isPlayerInMovingMode) _selectedIndex = -1;
    else {
        _selectedIndex = -1;
        float minDistance = std::numeric_limits<float>::infinity(); // Initialize with a large value

        auto camera = ComponentsManager::getInstance().GetComponentByID<Camera>(2);
        glm::vec3 cameraPosition = camera->GetPosition();
        glm::vec3 rayDirection = camera->GetFrontVector();

        for (int i = _turretIndexAtRestart; i < _newTurretIndex; i++) {
            std::string nameOfTurret = "Turret" + std::to_string(i);
            auto turret = NODESMANAGER.getNodeByName(nameOfTurret);
            auto meshRenderer = turret->GetComponent<MeshRenderer>();
            if (meshRenderer) {
                // Calculate distance from camera to turret center
                glm::vec3 turretCenter = turret->GetTransform()->GetGlobalPosition();
                float distance = glm::length(turretCenter - cameraPosition);

                // Check if this turret is closer than the current closest turret
                if (distance < minDistance) {
                    if (FrustumCulling::RayInBox(cameraPosition, rayDirection, meshRenderer->_model, turret->GetTransform()->GetGlobalCTM())) {
                        minDistance = distance;
                        _selectedIndex = i;
                    }
                }
            }
        }
    }
}

bool TurretsManager::IsSelectedTurretInRange() {
    if (_selectedIndex != -1 && _selectedIndex >= _turretIndexAtRestart) {
        std::string nameOfTurret = "Turret" + std::to_string(_selectedIndex);

        auto turretPosition = NODESMANAGER.getNodeByName(nameOfTurret)->GetTransform()->GetPosition();
        auto playerPosition = _player->GetTransform()->GetPosition();

        return glm::distance(turretPosition, playerPosition) + 0.01 <= 3;
    }
    return false;
}

void TurretsManager::Reset() {
    for (int i = 0; i < _turrets.size(); i++) {
        if (_turrets[i] == nullptr) continue;
        if(_turrets[i]->_flare != nullptr){
            NODESMANAGER.getNodeByName("root")->RemoveChild(_turrets[i]->_flare);
        }
        NODESMANAGER.getNodeByName("root")->RemoveChild(_turrets[i]->_ownerNode);
        _turrets[i] = nullptr;
    }
    //_turrets.clear();
    _turretCosts = { 1,1,1 };
    _isPlayerInMovingMode = false;
    PAGEMANAGER._PDAPage->HidePDAPage();
    _isInTurretChoiceMenu = false;
    _PDAController->HideImmediately();
    HideBlueprintTurret();
    _turretIndexAtRestart = _newTurretIndex;

}
