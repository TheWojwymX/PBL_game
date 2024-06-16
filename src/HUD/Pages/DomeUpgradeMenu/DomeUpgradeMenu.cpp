#include "DomeUpgradeMenu.h"
#include "Managers/UpgradeManager.h"

void DomeUpgradeMenu::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetCorners(CoordsConverter::ConvertCoords(glm::vec2(729, 732)), CoordsConverter::ConvertCoords(glm::vec2(1190, 347)));
    SetupButtons();
    _maxHPUpgradeButton->Init();
    _HPRegenButton->Init();
    _repairButton->Init();
    Page::Init();
}

void DomeUpgradeMenu::Update() {
    if (_shouldRender) {
        Page::Update();
        _maxHPUpgradeButton->Update();
        _HPRegenButton->Update();
        _repairButton->Update();
    }
}

void DomeUpgradeMenu::SetupButtons() {
    _maxHPUpgradeButton = std::make_shared<Button>("res/Images/UpgradeMenus/base_maxhp.png",
                                                   "res/Images/UpgradeMenus/base_maxhp_hover.png",
                                                   "res/Images/UpgradeMenus/base_maxhp_clicked.png",
                                                   CoordsConverter::ConvertCoords(glm::vec2(781, 471)),
                                                   CoordsConverter::ConvertCoords(glm::vec2(1138, 370)),
                                                   [this]() {
                                                       UPGRADEMANAGER.UpgradeDomeHp();
                                                   });

    _HPRegenButton = std::make_shared<Button>("res/Images/UpgradeMenus/base_maxhp.png",
                                              "res/Images/UpgradeMenus/base_maxhp_hover.png",
                                              "res/Images/UpgradeMenus/base_maxhp_clicked.png",
                                              CoordsConverter::ConvertCoords(glm::vec2(781, 590)),
                                              CoordsConverter::ConvertCoords(glm::vec2(1138, 489)),
                                              [this]() {
                                                  UPGRADEMANAGER.UpgradeDomeHPRegen();
                                              });

    _repairButton = std::make_shared<Button>("res/Images/UpgradeMenus/base_maxhp.png",
                                             "res/Images/UpgradeMenus/base_maxhp_hover.png",
                                             "res/Images/UpgradeMenus/base_maxhp_clicked.png",
                                             CoordsConverter::ConvertCoords(glm::vec2(781, 709)),
                                             CoordsConverter::ConvertCoords(glm::vec2(1138, 608)),
                                             [this]() {
                                                 UPGRADEMANAGER.DomeHPRepair();
                                             });
}