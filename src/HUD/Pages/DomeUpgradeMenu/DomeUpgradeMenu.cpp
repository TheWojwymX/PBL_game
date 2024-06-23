#include "DomeUpgradeMenu.h"
#include "Managers/UpgradeManager.h"

void DomeUpgradeMenu::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetCorners(CoordsConverter::ConvertCoords(glm::vec2(659, 732)), CoordsConverter::ConvertCoords(glm::vec2(1260, 347)));
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
        RenderText();
    }
}

void DomeUpgradeMenu::SetupButtons() {
    _maxHPUpgradeButton = std::make_shared<Button>("res/Images/UpgradeMenus/base_maxhp.png",
                                                   "res/Images/UpgradeMenus/base_maxhp_hover.png",
                                                   "res/Images/UpgradeMenus/base_maxhp_clicked.png",
                                                   CoordsConverter::ConvertCoords(glm::vec2(682, 471)),
                                                   CoordsConverter::ConvertCoords(glm::vec2(1237, 370)),
                                                   [this]() {
                                                       UPGRADEMANAGER.UpgradeDomeHp();
                                                   });

    _HPRegenButton = std::make_shared<Button>("res/Images/UpgradeMenus/base_hpregen.png",
                                              "res/Images/UpgradeMenus/base_hpregen_hover.png",
                                              "res/Images/UpgradeMenus/base_hpregen_clicked.png",
                                              CoordsConverter::ConvertCoords(glm::vec2(682, 590)),
                                              CoordsConverter::ConvertCoords(glm::vec2(1237, 489)),
                                              [this]() {
                                                  UPGRADEMANAGER.UpgradeDomeHPRegen();
                                              });

    _repairButton = std::make_shared<Button>("res/Images/UpgradeMenus/base_repair.png",
                                             "res/Images/UpgradeMenus/base_repair_hover.png",
                                             "res/Images/UpgradeMenus/base_repair_clicked.png",
                                             CoordsConverter::ConvertCoords(glm::vec2(682, 709)),
                                             CoordsConverter::ConvertCoords(glm::vec2(1237, 608)),
                                             [this]() {
                                                 UPGRADEMANAGER.DomeHPRepair();
                                             });
}

void DomeUpgradeMenu::RenderText(){
    glm::vec2 coords;
    string text;

    //levels
    coords = CoordsConverter::ConvertTextCoords(glm::vec2(788, 423));
    text = to_string(DOMEMANAGER.GetDomeHPLevel());
    TEXTRENDERER.RenderTextCentered(text, coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

    coords = CoordsConverter::ConvertTextCoords(glm::vec2(788, 541));
    text = to_string(DOMEMANAGER.GetDomeHPRegenLevel());
    TEXTRENDERER.RenderTextCentered(text, coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

    //costs
    coords = CoordsConverter::ConvertTextCoords(glm::vec2(1144, 423));
    if(UPGRADEMANAGER._domeHPUpgrades.upgradeCosts.size() == DOMEMANAGER.GetDomeHPLevel()){
        text = "MAX";
    }else{
        text = to_string(UPGRADEMANAGER._domeHPUpgrades.upgradeCosts[DOMEMANAGER.GetDomeHPLevel()]);
    }
    TEXTRENDERER.RenderTextCentered(text, coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

    coords = CoordsConverter::ConvertTextCoords(glm::vec2(1144, 541));
    if(UPGRADEMANAGER._domeHPRegenUpgrades.upgradeCosts.size() == DOMEMANAGER.GetDomeHPRegenLevel()){
        text = "MAX";
    }else{
        text = to_string(UPGRADEMANAGER._domeHPRegenUpgrades.upgradeCosts[DOMEMANAGER.GetDomeHPRegenLevel()]);
    }
    TEXTRENDERER.RenderTextCentered(text, coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

    coords = CoordsConverter::ConvertTextCoords(glm::vec2(1144, 660));
    TEXTRENDERER.RenderTextCentered(to_string(UPGRADEMANAGER._domeHPRepair.upgradeCosts[0]), coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
}