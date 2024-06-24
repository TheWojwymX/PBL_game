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
    _mainMenuPage->Init();
    _pages.push_back(_mainMenuPage);
    _mainMenuSettingsPage->Init();
    _pages.push_back(_mainMenuSettingsPage);
    _mainMenuControlsPage->Init();
    _pages.push_back(_mainMenuControlsPage);
    _restartPage->Init();
    _pages.push_back(_restartPage);

    CloseAllOtherPages(_mainMenuPage);
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
    _restartPage->Update();
    _mainMenuPage->Update();
    _mainMenuSettingsPage->Update();
    _mainMenuControlsPage->Update();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

}

void PageManager::CheckInputs() {

    if(GAMEMANAGER._isInMainMenu == true) return;

    if (INPUT.IsKeyPressed(GLFW_KEY_ESCAPE) && !_isInPage) {
        _pauseMenuPage->_shouldRender = !_pauseMenuPage->_shouldRender;
        if (_pauseMenuPage->_shouldRender) {
            CloseAllOtherPages(_pauseMenuPage);
            GAMEMANAGER.Pause();
        } else {
            GAMEMANAGER.Unpause();
            _isInUpgradeMenu = false;
        }
    } else if (INPUT.IsKeyPressed(GLFW_KEY_ESCAPE) && _isInPage) {
        _isInPage = false;
        CloseAllPages();
        if(TURRETSMANAGER._isInTurretChoiceMenu){
            NODESMANAGER.getNodeByName("PDA")->GetComponent<PDAController>()->HideImmediately();
        }
        GAMEMANAGER.Unpause();
        _isInUpgradeMenu = false;
    }

    if (INPUT.IsKeyPressed(GLFW_KEY_E) && !_pauseMenuPage->_shouldRender && UPGRADEMANAGER.IsPlayerStationInRange() &&
        UPGRADEMANAGER.IsPlayerStationInRaycast() && NODESMANAGER.getNodeByName("PDA")->GetComponent<PDAController>()->_isHidden) {
        if (!_isInPage) {
            _isInPage = true;
            _playerUpgradeMenu->_shouldRender = true;
            GAMEMANAGER.EnableMouse();
            HUD._shouldShowCrosshair = false;
            _isInUpgradeMenu = true;
        }
        else if(TUTORIALMANAGER._actualMessage == 8){
            CloseAllPages();
            _isInPage = true;
            _playerUpgradeMenu->_shouldRender = true;
            GAMEMANAGER.EnableMouse();
            HUD._shouldShowCrosshair = false;
            _isInUpgradeMenu = true;
        }
        else {
            _isInPage = false;
            CloseAllPages();
            GAMEMANAGER.DisableMouse();
            HUD._shouldShowCrosshair = true;
            _isInUpgradeMenu = false;
        }
        //it has to be this order because isTurretInRange won't work with index -1
    } else if (INPUT.IsKeyPressed(GLFW_KEY_E) && !_pauseMenuPage->_shouldRender && UPGRADEMANAGER.IsTurretInRaycast() && UPGRADEMANAGER.IsTurretInRange()) {
        if (!_isInPage) {
            _isInPage = true;
            _turretUpgradeMenu->_shouldRender = true;
        }else {
            _isInPage = false;
            CloseAllPages();
        }
    } else if (INPUT.IsKeyPressed(69) && !_pauseMenuPage->_shouldRender && UPGRADEMANAGER.IsDomeStationInRange() &&
        UPGRADEMANAGER.IsDomeStationInRaycast() && NODESMANAGER.getNodeByName("PDA")->GetComponent<PDAController>()->_isHidden) {
        if (!_isInPage) {
            _isInPage = true;
            _domeUpgradeMenu->_shouldRender = true;
            GAMEMANAGER.EnableMouse();
            HUD._shouldShowCrosshair = false;
            _isInUpgradeMenu = true;
        }
        else if(TUTORIALMANAGER._actualMessage == 8){
            CloseAllPages();
            _isInPage = true;
            _playerUpgradeMenu->_shouldRender = true;
            GAMEMANAGER.EnableMouse();
            HUD._shouldShowCrosshair = false;
            _isInUpgradeMenu = true;
        }
        else {
            _isInPage = false;
            CloseAllPages();
            GAMEMANAGER.DisableMouse();
            HUD._shouldShowCrosshair = true;
            _isInUpgradeMenu = false;
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
            page->_shouldRender = true;
            continue;
        } else {
            page->_shouldRender = false;
        }
    }
}

void PageManager::Reset() {
    CloseAllPages();
    _isInUpgradeMenu = false;
}

void PageManager::GoToSettingsMainMenu() {
    CloseAllOtherPages(_mainMenuSettingsPage);
}

void PageManager::GoToMainMenu() {
    CloseAllOtherPages(_mainMenuPage);
}

void PageManager::GoToKeyboardSettings() {
    CloseAllOtherPages(_mainMenuControlsPage);
}
