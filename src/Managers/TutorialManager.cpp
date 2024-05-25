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
    _dialogPage = PAGEMANAGER._dialogPage;
    _dialogPage->_shouldRender = true;
    _dialogPage->_actualText = _dialog1;
}

void TutorialManager::Update() {

}
