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
    _dialogPage = PAGEMANAGER._messagePage;
    _player = NODESMANAGER.getNodeByName("player");
    _paratrooper = NODESMANAGER.getNodeByName("Paratrooper");
    _paratrooper->GetComponent<MeshRenderer>()->_disableModel = true;
    NODESMANAGER.getNodeByName("waveSymbol1")->GetTransform()->SetPosition(glm::vec3(
            ENEMIESMANAGER._spawnersPositions[0][0], 320, ENEMIESMANAGER._spawnersPositions[0][1]));
    _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;
}

void TutorialManager::Update() {

    //std::cout << "tutorial skonczony: " << _isTutorialEnded << "   aktualna wiadomosc: " << _actualMessage << std::endl;

    if(_isTutorialEnded) return;

    switch (_actualMessage) {

        //konczy sie po wyladowaniu
        case 0:
            _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;
            if(_player->GetComponent<PlayerController>()->_isGrounded){
               DisplayAndChangeMessage();
               _player->GetComponent<PlayerController>()->SetGravity(-20.0f);
               NODESMANAGER.getNodeByName("root")->removeChild(_paratrooper);
                HUD._shouldShowMaterials = true;
                HUD._shouldShowCrosshair = true;
            }
            else{
                _player->GetTransform()->SetPosition(glm::vec3(_player->GetTransform()->GetPosition().x,
                                                               _player->GetTransform()->GetPosition().y - 0.01,
                                                               _player->GetTransform()->GetPosition().z));
                _paratrooper->GetTransform()->SetPosition(_player->GetTransform()->GetPosition());

                if(_timer < 1 && !_firstEnemySpawned){
                    _timer += TIME.GetDeltaTime();
                }else if(!_firstEnemySpawned){
                    _firstEnemySpawned = true;
                    SpawnTutorialEnemies(0);
                    ENEMIESMANAGER._enemies[0]->_walkingSpeed = ENEMIESMANAGER._enemies[0]->_walkingSpeed * 0.5;
                    _timer = 0;
                }
            }
            break;

        //konczy sie po kliknieciu L
        case 1:
            _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;
            if(INPUT.GetKeyDown(GLFW_KEY_L)){
                DisplayAndChangeMessage();
            }
            break;

        //konczy sie po pokonaniu mrowki
        case 2:
            _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;
            if(INPUT.GetMouseButtonDown(1)){
                PAGEMANAGER.CloseAllPages();
            }
            else if(ENEMIESMANAGER._enemies[0] == nullptr){
                SpawnTutorialEnemies(2);
                ENEMIESMANAGER._enemies[1]->_walkingSpeed = ENEMIESMANAGER._enemies[1]->_walkingSpeed * 0.6;
                HUD._shouldShowHP = true;
                DisplayAndChangeMessage();
            }

            break;

        //konczy sie po pokonaniu drugiej mrowki
        case 3:
            _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;
            if(TURRETSMANAGER._isPlayerInMovingMode) {
                PAGEMANAGER.CloseAllPages();
            }
            else if(ENEMIESMANAGER._enemies[1] == nullptr){
                DisplayAndChangeMessage();
                _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = false;
            }
            break;

        //konczy sie po dojsciu do podziemii
        case 4:
            if(_player->GetTransform()->GetPosition().y <= 288.5){
                DisplayAndChangeMessage();
            }
            break;

        //Konczy sie po rzuceniu flary
        case 5:
            if(_player->GetTransform()->GetPosition().y <= 288.5 && INPUT.GetMouseButtonDown(1)){
                DisplayAndChangeMessage();
            }
            else if(WarningSystem(0)){
                return;
            }
            break;

        //Konczy sie po wykopaniu materialow
        case 6:
            if(GAMEMANAGER._metal >= 1 && GAMEMANAGER._plastic >= 1){
                DisplayAndChangeMessage();
                HUD._shouldShowFuel = true;
            }
            else if(WarningSystem(1)){
                return;
            }
            break;

        //konczy sie po wyleceniu na powierzchnie
        case 7:
            if(_player->GetComponent<PlayerController>()->_isUsingJetpack){
                PAGEMANAGER.CloseAllPages();
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

        //konczy sie jak wejdzie na poziom -30
        case 10:
            if(_timer < 5){
                _timer += TIME.GetDeltaTime();
            }else{
                PAGEMANAGER.CloseAllPages();
                _timer = 0;
                _isFreePlay = true;
                if(_player->GetTransform()->GetPosition().y < GAMEMANAGER._groundLevel - 30){
                    DisplayAndChangeMessage();
                    HUD._shouldShowDepth = true;
                }
            }
            break;

        //konczy sie po 5 sekundach
        case 11:
            if(_timer < 5){
                _timer += TIME.GetDeltaTime();
            }else{
                _timer = 0;
                PAGEMANAGER.CloseAllPages();
                _isTutorialEnded = true;
            }
            break;
        default:
            break;
    }
}

void TutorialManager::SkipTutorial() {
    _isTutorialEnded = true;
    _isFreePlay = true;
    _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = false;
    _player->GetComponent<PlayerController>()->SetGravity(-20.0f);

    HUD._shouldShowCrosshair = true;
    HUD._shouldShowHP = true;
    HUD._shouldShowFuel = true;
    HUD._shouldShowMaterials = true;
    HUD._shouldShowDepth = true;
    HUD._shouldShowPhaseInfo = true;
}

void TutorialManager::SpawnTutorialEnemies(int spawnerIndex){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.5f, 1.0f);
    float scale = dis(gen);

    ENEMIESMANAGER.SpawnEnemy(2, glm::vec3(scale), spawnerIndex, ANT);
}

void TutorialManager::DisplayAndChangeMessage() {
    _dialogPage->_actualText = _messages[_actualMessage];
    PAGEMANAGER.DisplayMessagePage();

    //after all prepare next message
    if(_actualMessage < _messages.size() - 1){
        _actualMessage++;
    }
    else{
        _actualMessage = 11;
    }

    _isAfterWarning = false;
}

void TutorialManager::DisplaySpecialMessage(string message) {
    _dialogPage->_actualText = message;
    PAGEMANAGER.DisplayMessagePage();
}

bool TutorialManager::WarningSystem(int messageNumber) {

    if(glm::distance(_messagesPositions[messageNumber], _player->GetTransform()->GetPosition()) > 20 && !_isAfterWarning){
        DisplaySpecialMessage(_specialMessages[0]);
        _isAfterWarning = true;
        return true;
    }
    else if(glm::distance(_messagesPositions[messageNumber], _player->GetTransform()->GetPosition()) > 30){
        DisplaySpecialMessage(_specialMessages[1]);
        _isTutorialEnded = true;
        return true;
    }

    return false;
}
