#include "UpgradeDamage.h"

void UpgradeDamage::Init() {
    SetText(_text, 1);
    Button::Init();
}

void UpgradeDamage::Update() {
    Button::Update();
}

void UpgradeDamage::Onclick() {
    UPGRADEMANAGER.UpgradeTurretDamage();
}

void UpgradeDamage::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}