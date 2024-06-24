//
// Created by Jacek on 09.05.2024.
//

#define PAGEMANAGER PageManager::getInstance()

#ifndef SANDBOX_PAGEMANAGER_H
#define SANDBOX_PAGEMANAGER_H

#include <vector>
#include <memory>
#include "HUD/Pages/Page.h"
#include "HUD/Pages/PauseMenu/PauseMenuPage.h"
#include "HUD/Pages/PlayerUpgradeMenu/PlayerUpgradeMenu.h"
#include "HUD/Pages/TurretUpgradeMenu/TurretUpgradeMenu.h"
#include "HUD/Pages/DomeUpgradeMenu/DomeUpgradeMenu.h"
#include "HUD/Pages/MessagePage/MessagePage.h"
#include "HUD/Pages/PDA/PDAPage.h"
#include "HUD/Pages/RestartPage/RestartPage.h"
#include "HUD/Pages/MainMenuPage/MainMenuPage.h"
#include "HUD/Pages/MainMenuSettings/MainMenuSettingsPage.h"
#include "HUD/Pages/MainMenuControls/MainMenuControls.h"

class PageManager {
public:

    ~PageManager() = default;

    PageManager() = default;

    static PageManager &getInstance();

    PageManager(const PageManager &) = delete;

    PageManager &operator=(const PageManager &) = delete;

    void Init();
    void Update();

    std::vector<std::shared_ptr<Page>> _pages;
    std::shared_ptr<PauseMenuPage> _pauseMenuPage = std::make_shared<PauseMenuPage>();
    bool _isInPage = false;
    std::shared_ptr<PlayerUpgradeMenu> _playerUpgradeMenu = std::make_shared<PlayerUpgradeMenu>();
    std::shared_ptr<TurretUpgradeMenu> _turretUpgradeMenu = std::make_shared<TurretUpgradeMenu>();
    std::shared_ptr<DomeUpgradeMenu> _domeUpgradeMenu = std::make_shared<DomeUpgradeMenu>();

    std::shared_ptr<PDAPage> _PDAPage = std::make_shared<PDAPage>();

    std::shared_ptr<MainMenuPage> _mainMenuPage = std::make_shared<MainMenuPage>();

    std::shared_ptr<MainMenuSettingsPage> _mainMenuSettingsPage = std::make_shared<MainMenuSettingsPage>();

    std::shared_ptr<MainMenuControls> _mainMenuControlsPage = std::make_shared<MainMenuControls>();

    std::shared_ptr<RestartPage> _restartPage = std::make_shared<RestartPage>();


    void CheckInputs();

    void CloseAllOtherPages(const shared_ptr<Page>& pageException);

    void CloseAllPages();

    bool _isInUpgradeMenu = false;

    void Reset();

    void GoToSettingsMainMenu();

    void GoToMainMenu();

    void GoToKeyboardSettings();

};


#endif //SANDBOX_PAGEMANAGER_H
