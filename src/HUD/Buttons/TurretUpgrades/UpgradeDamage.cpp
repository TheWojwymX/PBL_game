//
// Created by Jacek on 21.05.2024.
//

#include "UpgradeDamage.h"

void UpgradeDamage::Init() {
    SetVertices(_buttonVertices);
    SetText(_text, 1);
    Button::Init();
}

void UpgradeDamage::Update() {
    Button::Update();
}

void UpgradeDamage::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void UpgradeDamage::Onclick() {
    UPGRADEMANAGER.UpgradeTurretDamage();
}

void UpgradeDamage::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}