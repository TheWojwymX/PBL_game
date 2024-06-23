//
// Created by Jacek on 25.05.2024.
//

#include "TutorialManager.h"
#include "HUD/PageManager.h"
#include "Enemies/EnemiesManager.h"
#include "Turrets/TurretsManager.h"

TutorialManager &TutorialManager::getInstance() {
    static TutorialManager instance;
    return instance;
}

void TutorialManager::Init() {
    _player = NODESMANAGER.getNodeByName("player");
    _paratrooper = NODESMANAGER.getNodeByName("Paratrooper");
    _paratrooper->GetComponent<MeshRenderer>()->_disableModel = true;
    _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;
    HUD._actualText = _messages[_actualMessage];
}

void TutorialManager::Update() {
    //std::cout << "tutorial skonczony: " << _isTutorialEnded << "   aktualna wiadomosc: " << _actualMessage << std::endl;
    if(_isTutorialEnded) return;

    if(_isSpecialMessage){
        if(_specialMessageTimer < 5){
            _specialMessageTimer += TIME.GetDeltaTime();
            //std::cout << _specialMessageTimer << std::endl;
        }else{
            _specialMessageTimer = 0.0f;
            _isSpecialMessage = false;
            if(_actualMessage <= 9){
                HUD._actualText = _messages[_actualMessage - 1];
            }else{
                SetFalseTutorialNeededAtMoment();
            }
        }
    }

    switch (_actualMessage) {
        //konczy sie po wyladowaniu
        case 0:
            ControllHud(0,0,0,0,0,0);
            _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;
            if(_player->GetComponent<PlayerController>()->_isGrounded){
               DisplayAndChangeMessage();
               _player->GetComponent<PlayerController>()->SetGravity(-20.0f);
               _paratrooper->GetComponent<MeshRenderer>()->_disableShadows = true;
                HUD._shouldShowCrosshair = true;
                HUD._shouldShowMaterials = true;
            }
            else{
                if(!GAMEMANAGER._paused) {
                    _player->GetTransform()->SetPosition(glm::vec3(_player->GetTransform()->GetPosition().x,
                                                                   _player->GetTransform()->GetPosition().y - TIME.GetDeltaTime() * 2,
                                                                   _player->GetTransform()->GetPosition().z));
                    _paratrooper->GetTransform()->SetPosition(_player->GetTransform()->GetPosition());
                    if (_timer < 1 && !_firstEnemySpawned) {
                        _timer += TIME.GetDeltaTime();
                    } else if (!_firstEnemySpawned) {
                        _firstEnemySpawned = true;
                        SpawnTutorialEnemies(0);
                        _timer = 0;
                        _player->GetComponent<PlayerAudioController>()->ChangeMusicToBattlePhase();
                    }
                }
            }
            break;

        //konczy sie po przejsciu do wyboru pozycji
        case 1:
            if(INPUT.IsKeyPressed(GLFW_KEY_R)){
                SkipTutorial();
                break;
            }

            ControllHud(1,1,0,0,0,0);
            _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;

            if(TURRETSMANAGER._isInBlueprintMode){
                DisplayAndChangeMessage();
            }
            break;

        //konczy sie po pokonaniu mrowki
        case 2:
            ControllHud(1,1,0,0,0,0);
            _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;
            if(!TURRETSMANAGER._isInBlueprintMode && HUD._isTutorialNeededAtMoment == true){
                SetFalseTutorialNeededAtMoment();
            }

            else if(ENEMIESMANAGER._enemies[0] == nullptr){
                SpawnTutorialEnemies(1);
                HUD._shouldShowHP = true;
                DisplayAndChangeMessage();
            }

            break;

        //konczy sie po pokonaniu drugiej mrowki
        case 3:
            ControllHud(1,1,1,0,0,0);
            _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;
            if(TURRETSMANAGER._isPlayerInMovingMode && HUD._isTutorialNeededAtMoment) {
                SetFalseTutorialNeededAtMoment();
            }
            else if(ENEMIESMANAGER._enemies[1] == nullptr){
                DisplayAndChangeMessage();
                _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = false;
                RESOURCEMANAGER.GetSoundByName("VictoryMusic")->RiseUp(2, NODESMANAGER.getNodeByName("player"));
                _player->GetComponent<PlayerAudioController>()->ChangeMusicToDigPhase();
            }
            break;

        //konczy sie po dojsciu do podziemii
        case 4:
            ControllHud(1,1,1,0,0,0);
            if(_player->GetTransform()->GetPosition().y <= 288.5){
                DisplayAndChangeMessage();
            }
            break;

        //Konczy sie po rzuceniu flary
        case 5:
            ControllHud(1,1,1,0,0,0);
            if(_player->GetTransform()->GetPosition().y <= 288.5 && INPUT.GetMouseButtonDown(1)){
                DisplayAndChangeMessage();
            }
            else if(WarningSystem(0)){
                return;
            }
            break;

        //Konczy sie po wykopaniu materialow
        case 6:
            ControllHud(1,1,1,0,0,0);
            if(GAMEMANAGER.GetPlastic() >= 5) {
                DisplayAndChangeMessage();
                HUD._shouldShowFuel = true;
            }
            else if(WarningSystem(1)){
                return;
            }
            break;

        //konczy sie po wyleceniu na powierzchnie
        case 7:
            ControllHud(1,1,1,1,0,0);
            if(_player->GetComponent<PlayerController>()->_isUsingJetpack){
                SetFalseTutorialNeededAtMoment();
            }
            else if(_player->GetTransform()->GetPosition().y >= 299.5 && _player->GetComponent<PlayerController>()->_isGrounded){
                DisplayAndChangeMessage();
            }
            else if(_player->GetTransform()->GetPosition().y < 299.5){
                if(WarningSystem(1)) {
                    return;
                }
            }
            break;

        //konczy sie jak wejdzie w interakcje ze stolem
        case 8:
            ControllHud(1,1,1,1,0,0);
            if(PAGEMANAGER._domeUpgradeMenu->_shouldRender || PAGEMANAGER._playerUpgradeMenu->_shouldRender){
                DisplayAndChangeMessage();
            }
            else if(_player->GetTransform()->GetPosition().y < 299.5){
                if(WarningSystem(1)) {
                    return;
                }
            }
            break;

        //konczy sie jak wyjdzie ze stolu
        case 9:
            ControllHud(1,1,1,1,0,0);
            if(PAGEMANAGER._isInPage == false){
                DisplayAndChangeMessage();
                HUD._shouldShowPhaseInfo = true;
            }
            else if(_player->GetTransform()->GetPosition().y < 299.5){
                if(WarningSystem(1)) {
                    return;
                }
            }
            break;

        //konczy sie jak oddali sie od dziury na odleglosc 30
        case 10:
            CheckEnemiesEncounter();
            ControllHud(1,1,1,1,1,0);
            if(_timer < 5){
                _timer += TIME.GetDeltaTime();
            }else{
                SetFalseTutorialNeededAtMoment();
                _timer = 0;
                _isFreePlay = true;
                if(_player->GetTransform()->GetPosition().y < GAMEMANAGER._groundLevel && glm::distance(_player->GetTransform()->GetPosition(),
                                  glm::vec3(GAMEMANAGER._domePosition.x, GAMEMANAGER._groundLevel, GAMEMANAGER._domePosition.y)) > 30){
                    DisplayAndChangeMessage();
                    HUD._shouldShowDepth = true;
                }
            }
            break;

        //konczy sie po 5 sekundach
        case 11:
            CheckEnemiesEncounter();
            ControllHud(1,1,1,1,1,1);
            if(_timer < 5){
                _timer += TIME.GetDeltaTime();
            }else{
                SetFalseTutorialNeededAtMoment();
                if(NODESMANAGER.getNodeByName("CompassNode")->GetComponent<CompassController>()->IsWithinHoleRange() &&
                   !NODESMANAGER.getNodeByName("CompassNode")->GetComponent<CompassController>()->_isHidden){
                    _timer = 0;
                    DisplayAndChangeMessage();
                }
            }
            break;
        //konczy sie po 5 sekundach
        case 12:
            CheckEnemiesEncounter();
            if(_timer < 5){
                _timer += TIME.GetDeltaTime();
            }else{
                _timer = 0;
                SetFalseTutorialNeededAtMoment();
                if(_metWasp && _metBeetle){
                    _isTutorialEnded = true;
                }
            }
            break;
        default:
            break;
    }
}

void TutorialManager::SetFalseTutorialNeededAtMoment(){
    if(_specialMessageTimer <= 0){
        HUD._isTutorialNeededAtMoment = false;
    }
}

void TutorialManager::CheckEnemiesEncounter(){

    //std::cout << "sprawdza warunek przeciwnikow" << std::endl;

    if(!_metBeetle){
        for(shared_ptr<Enemy> enemy : ENEMIESMANAGER._enemies){
            if(enemy == nullptr) continue;
            if(enemy->_enemyType == BEETLE){
                _metBeetle = true;
                DisplaySpecialMessage(_specialMessages[1]);
                //std::cout << "zuk spotkany, czas timer to: "<< _specialMessageTimer << std::endl;
            }
        }
    }
    if(!_metWasp){
        for(shared_ptr<Enemy> enemy : ENEMIESMANAGER._enemies){
            if(enemy == nullptr) continue;
            if(enemy->_enemyType == WASP){
                _metWasp = true;
                DisplaySpecialMessage(_specialMessages[2]);
                //std::cout << "osa spotkana, czas timer to: "<< _specialMessageTimer << std::endl;
            }
        }
    }
}


void TutorialManager::SkipTutorial() {
    _isTutorialEnded = true;
    _isFreePlay = true;
    _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = false;
    _player->GetComponent<PlayerController>()->SetGravity(-20.0f);

    HUD._isTutorialNeededAtMoment = false;

    HUD._isAfterTutorialCrosshair = true;
    HUD._isAfterTutorialHP = true;
    HUD._isAfterTutorialFuel = true;
    HUD._isAfterTutorialMaterials = true;
    HUD._isAfterTutorialDepth = true;
    HUD._isAfterTutorialPhaseInfo = true;

    HUD.EnableHUD();

    _paratrooper->GetComponent<MeshRenderer>()->_disableShadows = true;

    _metBeetle = true;
    _metWasp = true;

    ENEMIESMANAGER.Reset();

    _player->GetComponent<PlayerAudioController>()->ChangeMusicToDigPhase();
}

void TutorialManager::SpawnTutorialEnemies(int spawnerIndex){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.5f, 1.0f);
    float scale = dis(gen);

    ENEMIESMANAGER.SpawnEnemy(2, glm::vec3(scale), ENEMIESMANAGER.GetSpawnerPos(spawnerIndex), ANT);
}

void TutorialManager::DisplayAndChangeMessage() {
    HUD._isTutorialNeededAtMoment = true;
    HUD._shouldShowTutorial = true;
    HUD._actualText = _messages[_actualMessage];

    //after all prepare next message
    if(_actualMessage < _messages.size() - 1){
        _actualMessage++;
    }
    else{
        _actualMessage = 12;
    }
    _isAfterWarning = false;
}

void TutorialManager::DisplaySpecialMessage(string message) {
    _isSpecialMessage = true;
    HUD._actualText = message;
    HUD._isTutorialNeededAtMoment = true;
    HUD._shouldShowTutorial = true;
    _specialMessageTimer = 0.0f;
}

bool TutorialManager::WarningSystem(int messageNumber) {

    if(glm::distance(_messagesPositions[messageNumber], _player->GetTransform()->GetPosition()) > 20 && !_isAfterWarning){
        DisplaySpecialMessage(_specialMessages[0]);
        _isAfterWarning = true;
        return true;
    }
    else if(glm::distance(_messagesPositions[messageNumber], _player->GetTransform()->GetPosition()) > 30){
        SkipTutorial();
        return true;
    }

    return false;
}

void TutorialManager::ControllHud(bool crosshair, bool materials, bool hp, bool fuel, bool phase, bool depth){
    HUD._isAfterTutorialMaterials = materials;
    HUD._isAfterTutorialCrosshair = crosshair;
    HUD._isAfterTutorialHP = hp;
    HUD._isAfterTutorialFuel = fuel;
    HUD._isAfterTutorialDepth = depth;
    HUD._isAfterTutorialPhaseInfo = phase;
}

void TutorialManager::Reset() {
    _player->GetTransform()->SetPosition(glm::vec3(49.5, 305.5, 60));
    _firstEnemySpawned = false;
    _isFreePlay = false;
    _isAfterWarning = false;
    _isTutorialEnded = false;
    HUD._isAfterTutorialMaterials = false;
    HUD._isAfterTutorialCrosshair = false;
    HUD._isAfterTutorialHP = false;
    HUD._isAfterTutorialFuel = false;
    HUD._isAfterTutorialDepth = false;
    HUD._isAfterTutorialPhaseInfo = false;
    HUD._isTutorialNeededAtMoment = false;
    HUD._shouldShowTutorial = false;
    _actualMessage = 0;
    _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;
    HUD._actualText = _messages[_actualMessage];
    _player->GetComponent<PlayerController>()->_isGrounded = false;
    _player->GetComponent<PlayerController>()->SetGravity(0);
    _paratrooper->GetComponent<MeshRenderer>()->_disableShadows = false;
    _player->GetComponent<PlayerController>()->_velocity.y = 0;
    _metBeetle = false;
    _metWasp = false;
}
