#include "PlayerUpgradeMenu.h"
#include "Managers/UpgradeManager.h"

void PlayerUpgradeMenu::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetCorners(CoordsConverter::ConvertCoords(glm::vec2(659, 794)), CoordsConverter::ConvertCoords(glm::vec2(1260, 285)));
    SetupButtons();
    _capacityUpgradeButton->Init();
    _reachUpgradeButton->Init();
    _radiusUpgradeButton->Init();
    _speedUpgradeButton->Init();
    _playerController = NODESMANAGER.getNodeByName("player")->GetComponent<PlayerController>();
    Page::Init();
}

void PlayerUpgradeMenu::Update() {
    if (_shouldRender) {
        Page::Update();
        _capacityUpgradeButton->Update();
        _reachUpgradeButton->Update();
        _radiusUpgradeButton->Update();
        _speedUpgradeButton->Update();
        RenderText();
    }
}

void PlayerUpgradeMenu::SetupButtons() {
    _capacityUpgradeButton = std::make_shared<Button>("res/Images/UpgradeMenus/jetpack_capacity.png",
                                                      "res/Images/UpgradeMenus/jetpack_capacity_hover.png",
                                                      "res/Images/UpgradeMenus/jetpack_capacity_clicked.png",
                                                      CoordsConverter::ConvertCoords(glm::vec2(682, 411)),
                                                      CoordsConverter::ConvertCoords(glm::vec2(1237, 310)),
                                                      [this]() {
                                                       UPGRADEMANAGER.UpgradeJetpackCapacity();
                                                   });

    _reachUpgradeButton = std::make_shared<Button>("res/Images/UpgradeMenus/mining_reach.png",
                                                   "res/Images/UpgradeMenus/mining_reach_hover.png",
                                                   "res/Images/UpgradeMenus/mining_reach_clicked.png",
                                                   CoordsConverter::ConvertCoords(glm::vec2(682, 530)),
                                                   CoordsConverter::ConvertCoords(glm::vec2(1237, 429)),
                                                   [this]() {
                                                  UPGRADEMANAGER.UpgradeMiningReach();
                                              });

    _radiusUpgradeButton = std::make_shared<Button>("res/Images/UpgradeMenus/mining_radius.png",
                                                    "res/Images/UpgradeMenus/mining_radius_hover.png",
                                                    "res/Images/UpgradeMenus/mining_radius_clicked.png",
                                                    CoordsConverter::ConvertCoords(glm::vec2(682, 649)),
                                                    CoordsConverter::ConvertCoords(glm::vec2(1237, 548)),
                                                    [this]() {
                                                 UPGRADEMANAGER.UpgradeMiningRadius();
                                             });

    _speedUpgradeButton = std::make_shared<Button>("res/Images/UpgradeMenus/mining_speed.png",
                                                    "res/Images/UpgradeMenus/mining_speed_hover.png",
                                                    "res/Images/UpgradeMenus/mining_speed_clicked.png",
                                                    CoordsConverter::ConvertCoords(glm::vec2(682, 769)),
                                                    CoordsConverter::ConvertCoords(glm::vec2(1237, 668)),
                                                    [this]() {
                                                 UPGRADEMANAGER.UpgradeMiningSpeed();
                                             });
}

void PlayerUpgradeMenu::RenderText(){
    glm::vec2 coords;
    string text;

    //levels
    coords = CoordsConverter::ConvertTextCoords(glm::vec2(788, 360));
    text = to_string(_playerController->GetJetpackCapacityLevel());
    TEXTRENDERER.RenderTextCentered(text, coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

    coords = CoordsConverter::ConvertTextCoords(glm::vec2(788, 479));
    text = to_string(_playerController->GetMiningReachLevel());
    TEXTRENDERER.RenderTextCentered(text, coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

    coords = CoordsConverter::ConvertTextCoords(glm::vec2(788, 598));
    text = to_string(_playerController->GetMiningRadiusLevel());
    TEXTRENDERER.RenderTextCentered(text, coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

    coords = CoordsConverter::ConvertTextCoords(glm::vec2(788, 718));
    text = to_string(_playerController->GetMiningSpeedLevel());
    TEXTRENDERER.RenderTextCentered(text, coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

    //costs
    coords = CoordsConverter::ConvertTextCoords(glm::vec2(1144, 360));
    if(UPGRADEMANAGER._jetpackCapacityUpgrades.upgradeCosts.size() == _playerController->GetJetpackCapacityLevel()){
        text = "MAX";
    }else{
        text = to_string(UPGRADEMANAGER._jetpackCapacityUpgrades.upgradeCosts[_playerController->GetJetpackCapacityLevel()]);
    }
    TEXTRENDERER.RenderTextCentered(text, coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

    coords = CoordsConverter::ConvertTextCoords(glm::vec2(1144, 479));
    if(UPGRADEMANAGER._miningReachUpgrades.upgradeCosts.size() == _playerController->GetMiningReachLevel()){
        text = "MAX";
    }else{
        text = to_string(UPGRADEMANAGER._miningReachUpgrades.upgradeCosts[_playerController->GetMiningReachLevel()]);
    }
    TEXTRENDERER.RenderTextCentered(text, coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

    coords = CoordsConverter::ConvertTextCoords(glm::vec2(1144, 598));
    if(UPGRADEMANAGER._miningRadiusUpgrades.upgradeCosts.size() == _playerController->GetMiningRadiusLevel()){
        text = "MAX";
    }else{
        text = to_string(UPGRADEMANAGER._miningRadiusUpgrades.upgradeCosts[_playerController->GetMiningRadiusLevel()]);
    }
    TEXTRENDERER.RenderTextCentered(text, coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

    coords = CoordsConverter::ConvertTextCoords(glm::vec2(1144, 718));
    if(UPGRADEMANAGER._miningSpeedUpgrades.upgradeCosts.size() == _playerController->GetMiningSpeedLevel()){
        text = "MAX";
    }else{
        text = to_string(UPGRADEMANAGER._miningSpeedUpgrades.upgradeCosts[_playerController->GetMiningSpeedLevel()]);
    }
    TEXTRENDERER.RenderTextCentered(text, coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

}