//
// Created by Jacek on 25.05.2024.
//

#include "TutorialManager.h"
#include "HUD/PageManager.h"

TutorialManager &TutorialManager::getInstance() {
    static TutorialManager instance;
    return instance;
}

void TutorialManager::Init() {
    _dialogPage = PAGEMANAGER._messagePage;
    DisplayAndChangeMessage();
    _player = NODESMANAGER.getNodeByName("player");

    NODESMANAGER.getNodeByName("waveSymbol1")->GetTransform()->SetPosition(glm::vec3(
            ENEMIESMANAGER._spawnersPositions[0][0], 320, ENEMIESMANAGER._spawnersPositions[0][1]));
}

void TutorialManager::Update() {

    if(_isTutorialEnded) return;

    switch (_actualMessage) {
        //po pokonaniu pierwszej fali sie konczy
        case 1:
            _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;

            if(!_firstWaveSpawned && !TURRETSMANAGER._turrets.empty()){

                if(_timer < 5){
                    _timer += TIME.GetDeltaTime();
                }else{
                    _firstWaveSpawned = true;
                    SpawnTutorialEnemies(0);
                    SpawnTutorialEnemies(0);
                    SpawnTutorialEnemies(0);
                    _timer = 0;
                }
            }
            else if(_firstWaveSpawned){
                if(ENEMIESMANAGER._enemies[0] == nullptr && ENEMIESMANAGER._enemies[1] == nullptr && ENEMIESMANAGER._enemies[2] == nullptr){
                    DisplayAndChangeMessage();
                    NODESMANAGER.getNodeByName("waveSymbol1")->GetTransform()->SetPosition(glm::vec3(
                            ENEMIESMANAGER._spawnersPositions[0][0], 0, ENEMIESMANAGER._spawnersPositions[0][1]));
                    NODESMANAGER.getNodeByName("waveSymbol2")->GetTransform()->SetPosition(glm::vec3(
                            ENEMIESMANAGER._spawnersPositions[1][0], 320, ENEMIESMANAGER._spawnersPositions[1][1]));
                }
            }
            break;

        //po pokonaniu 2 fali sie konczy
        case 2:
            _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = true;

            if(!_secondWaveSpawned){
                if(TURRETSMANAGER._isPlayerInMovingMode){
                    _isTimerOn = true;
                }
                if(_isTimerOn){
                    if(_timer < 5){
                        _timer += TIME.GetDeltaTime();
                    }else {
                        _secondWaveSpawned = true;
                        SpawnTutorialEnemies(1);
                        SpawnTutorialEnemies(1);
                        SpawnTutorialEnemies(1);
                        _timer = 0;
                    }
                }
            }
            else if(_secondWaveSpawned){
                if(ENEMIESMANAGER._enemies[3] == nullptr && ENEMIESMANAGER._enemies[4] == nullptr && ENEMIESMANAGER._enemies[5] == nullptr){
                    DisplayAndChangeMessage();
                    NODESMANAGER.getNodeByName("waveSymbol2")->GetTransform()->SetPosition(glm::vec3(ENEMIESMANAGER._spawnersPositions[1][0], 0,
                                                                                                     ENEMIESMANAGER._spawnersPositions[1][1]));
                    _player->GetComponent<PlayerController>()->_activeMineEntranceCollision = false;
                }
            }
            break;

        //konczy sie po dojsciu do podziemii
        case 3:
            if(glm::distance(_player->GetTransform()->GetPosition(), glm::vec3(49.15, 288.5, 49.8)) < 3 && _player->GetTransform()->GetPosition().y == 288.5){
                DisplayAndChangeMessage();
            }
            break;

        //Konczy sie po zdobyciu pieniedzy
        case 4:
            if(GAMEMANAGER._money == 100){
                DisplayAndChangeMessage();
            }
            else if(WarningSystem(0)){
                return;
            }
            break;

        //Konczy sie po dojsciu do dolu dziury
        case 5:
            if(glm::distance(_player->GetTransform()->GetPosition(), glm::vec3(49.15, 288.5, 49.8)) < 2 && _player->GetTransform()->GetPosition().y == 288.5){
                DisplayAndChangeMessage();
            }
            else if(WarningSystem(1)){
                return;
            }
            break;

        //konczy sie po wyleceniu na powierzchnie
        case 6:
            if(_player->GetTransform()->GetPosition().y >= 299.5 && _player->GetComponent<PlayerController>()->_isGrounded){
                DisplayAndChangeMessage();
            }
            else if(_player->GetTransform()->GetPosition().y < 299.5){
                if(WarningSystem(1)) {
                    return;
                }
            }
            break;

        //konczy sie jak paliwo jest na 100%
        case 7:
            if(_player->GetComponent<PlayerController>()->_jetpackFuel == _player->GetComponent<PlayerController>()->_maxJetpackFuel && PAGEMANAGER._isInPage == false){
                DisplayAndChangeMessage();
            }
            else if(_player->GetTransform()->GetPosition().y < 299.5){
                if(WarningSystem(1)) {
                    return;
                }
            }
            break;

        //konczy sie jak ulepszy szybkosc
        case 8:
            if(_player->GetComponent<PlayerController>()->_speed > 8.0 && PAGEMANAGER._isInPage == false){
                DisplayAndChangeMessage();
                _isTutorialEnded = true;
            }
            else if(_player->GetTransform()->GetPosition().y < 299.5){
                if(WarningSystem(1)) {
                    return;
                }
            }
            break;

        default:
            break;
    }
}

void TutorialManager::SpawnTutorialEnemies(int spawnerIndex){

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.5f, 1.0f);
    float scale = dis(gen);

    ENEMIESMANAGER.SpawnEnemy(2, glm::vec3(scale), spawnerIndex);
}

void TutorialManager::DisplayAndChangeMessage() {
    _dialogPage->_actualText = _messages[_actualMessage];
    PAGEMANAGER.DisplayMessagePage();

    //after all prepare next message
    if(_actualMessage < _messages.size() - 1){
        _actualMessage++;
    }
    else{
        _actualMessage = 2137;
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
