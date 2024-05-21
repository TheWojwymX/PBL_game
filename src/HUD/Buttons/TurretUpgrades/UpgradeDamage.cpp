//
// Created by Jacek on 21.05.2024.
//

#include "UpgradeDamage.h"

void UpgradeDamage::Init() {
    SetVertices(_buttonVertices);
    SetText(_text);
    Button::Init();
}

void UpgradeDamage::Update() {
    Button::Update();
}

void UpgradeDamage::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void UpgradeDamage::Onclick() {
    UPGRADEMANAGER.upgradeTurretDamage();
}

void UpgradeDamage::SetText(std::string text) {
    Button::SetText(text);
}