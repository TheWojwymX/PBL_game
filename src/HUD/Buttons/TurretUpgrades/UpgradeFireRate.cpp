#include "UpgradeFireRate.h"

void UpgradeFireRate::Init() {
    SetButtonPosition(glm::vec2(0,0));
    SetText(_text, 1);
    Button::Init();
}

void UpgradeFireRate::Update() {
    Button::Update();
}

void UpgradeFireRate::Onclick() {
    UPGRADEMANAGER.UpgradeTurretFireRate();
}

void UpgradeFireRate::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}