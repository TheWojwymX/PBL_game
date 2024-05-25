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
    DisplayMessage();
}

void TutorialManager::Update() {

    //for testing purposes display message every 10 seconds
    if(_tutorialTimer <= 10){
        _tutorialTimer += TIME.GetDeltaTime();
    }
    else{
        _tutorialTimer = 0.0f;
        DisplayMessage();
    }
}

void TutorialManager::DisplayMessage() {
    _dialogPage->_actualText = _messages[_actualMessage];
    PAGEMANAGER.DisplayMessagePage();

    //after all prepare next message
    if(_actualMessage < _messages.size() - 1){
        _actualMessage++;
    }
}
