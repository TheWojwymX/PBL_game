#include "PlayerUpgradeMenu.h"
#include "Managers/UpgradeManager.h"

void PlayerUpgradeMenu::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetCorners(CoordsConverter::ConvertCoords(glm::vec2(729, 794)), CoordsConverter::ConvertCoords(glm::vec2(1190, 285)));
    SetupButtons();
    _capacityUpgradeButton->Init();
    _reachUpgradeButton->Init();
    _radiusUpgradeButton->Init();
    _speedUpgradeButton->Init();
    Page::Init();
}

void PlayerUpgradeMenu::Update() {
    if (_shouldRender) {
        Page::Update();
        _capacityUpgradeButton->Update();
        _reachUpgradeButton->Update();
        _radiusUpgradeButton->Update();
        _speedUpgradeButton->Update();
    }
}

void PlayerUpgradeMenu::SetupButtons() {
    _capacityUpgradeButton = std::make_shared<Button>("res/Images/UpgradeMenus/jetpack_capacity.png",
                                                      "res/Images/UpgradeMenus/jetpack_capacity_hover.png",
                                                      "res/Images/UpgradeMenus/jetpack_capacity_clicked.png",
                                                      CoordsConverter::ConvertCoords(glm::vec2(781, 411)),
                                                      CoordsConverter::ConvertCoords(glm::vec2(1138, 310)),
                                                      [this]() {
                                                       UPGRADEMANAGER.UpgradeJetpackCapacity();
                                                   });

    _reachUpgradeButton = std::make_shared<Button>("res/Images/UpgradeMenus/mining_reach.png",
                                                   "res/Images/UpgradeMenus/mining_reach_hover.png",
                                                   "res/Images/UpgradeMenus/mining_reach_clicked.png",
                                                   CoordsConverter::ConvertCoords(glm::vec2(781, 530)),
                                                   CoordsConverter::ConvertCoords(glm::vec2(1138, 429)),
                                                   [this]() {
                                                  UPGRADEMANAGER.UpgradeMiningReach();
                                              });

    _radiusUpgradeButton = std::make_shared<Button>("res/Images/UpgradeMenus/mining_radius.png",
                                                    "res/Images/UpgradeMenus/mining_radius_hover.png",
                                                    "res/Images/UpgradeMenus/mining_radius_clicked.png",
                                                    CoordsConverter::ConvertCoords(glm::vec2(781, 649)),
                                                    CoordsConverter::ConvertCoords(glm::vec2(1138, 548)),
                                                    [this]() {
                                                 UPGRADEMANAGER.UpgradeMiningRadius();
                                             });

    _speedUpgradeButton = std::make_shared<Button>("res/Images/UpgradeMenus/mining_speed.png",
                                                    "res/Images/UpgradeMenus/mining_speed_hover.png",
                                                    "res/Images/UpgradeMenus/mining_speed_clicked.png",
                                                    CoordsConverter::ConvertCoords(glm::vec2(781, 769)),
                                                    CoordsConverter::ConvertCoords(glm::vec2(1138, 668)),
                                                    [this]() {
                                                 UPGRADEMANAGER.UpgradeMiningSpeed();
                                             });
}