//
// Created by Jacek on 09.05.2024.
//

#include "PageManager.h"
#include "Managers/ComponentsManager.h"
#include "Managers/GameManager.h"
#include "Managers/UpgradeManager.h"
#include "Managers/TutorialManager.h"

PageManager &PageManager::getInstance() {
    static PageManager instance;
    return instance;
}

void PageManager::Init() {
    _pauseMenuPage->Init();
    _pages.push_back(_pauseMenuPage);
    _playerUpgradeMenu->Init();
    _pages.push_back(_playerUpgradeMenu);
    _turretUpgradeMenu->Init();
    _pages.push_back(_turretUpgradeMenu);
    _domeUpgradeMenu->Init();
    _pages.push_back(_domeUpgradeMenu);
    _PDAPage->Init();
    _pages.push_back(_PDAPage);
}

void PageManager::Update() {
    CheckInputs();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    _pauseMenuPage->Update();
    _playerUpgradeMenu->Update();
    _turretUpgradeMenu->Update();
    _domeUpgradeMenu->Update();
    _PDAPage->Update();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

}

void PageManager::CheckInputs() {
    if (INPUT.IsKeyPressed(GLFW_KEY_ESCAPE) && !_isInPage) {
        _pauseMenuPage->_shouldRender = !_pauseMenuPage->_shouldRender;
        if (_pauseMenuPage->_shouldRender) {
            CloseAllOtherPages(_pauseMenuPage);
            GAMEMANAGER.Pause();
        } else {
            GAMEMANAGER.Unpause();
        }
    } else if (INPUT.IsKeyPressed(GLFW_KEY_ESCAPE) && _isInPage) {
        _isInPage = false;
        CloseAllPages();
        GAMEMANAGER.Unpause();
    }

    if (INPUT.IsKeyPressed(70) && !_pauseMenuPage->_shouldRender && UPGRADEMANAGER.IsPlayerStationInRange() && UPGRADEMANAGER.IsPlayerStationInRaycast()) {
        if (!_isInPage) {
            _isInPage = true;
            _playerUpgradeMenu->_shouldRender = true;
        }
        else if(TUTORIALMANAGER._actualMessage == 8){
            CloseAllPages();
            _isInPage = true;
            _playerUpgradeMenu->_shouldRender = true;
        }
        else {
            _isInPage = false;
            CloseAllPages();
        }
        //it has to be this order because isTurretInRange won't work with index -1
    } else if (INPUT.IsKeyPressed(70) && !_pauseMenuPage->_shouldRender && UPGRADEMANAGER.IsTurretInRaycast() && UPGRADEMANAGER.IsTurretInRange()) {
        if (!_isInPage) {
            _isInPage = true;
            _turretUpgradeMenu->_shouldRender = true;
        }
        else if(TUTORIALMANAGER._actualMessage == 8){
            CloseAllPages();
            _isInPage = true;
            _playerUpgradeMenu->_shouldRender = true;
        }else {
            _isInPage = false;
            CloseAllPages();
        }
    } else if (INPUT.IsKeyPressed(70) && !_pauseMenuPage->_shouldRender && UPGRADEMANAGER.IsDomeStationInRange() && UPGRADEMANAGER.IsDomeStationInRaycast()) {
        if (!_isInPage) {
            _isInPage = true;
            _domeUpgradeMenu->_shouldRender = true;
        }
        else if(TUTORIALMANAGER._actualMessage == 8){
            CloseAllPages();
            _isInPage = true;
            _playerUpgradeMenu->_shouldRender = true;
        }
        else {
            _isInPage = false;
            CloseAllPages();
        }
    }

}

void PageManager::CloseAllPages() {
    for (const std::shared_ptr<Page> &page: _pages) {
        page->_shouldRender = false;
    }
}

void PageManager::CloseAllOtherPages(const shared_ptr<Page> &pageException) {
    for (const std::shared_ptr<Page> &page: _pages) {
        if (pageException == page) {
            continue;
        } else {
            page->_shouldRender = false;
        }
    }
}
