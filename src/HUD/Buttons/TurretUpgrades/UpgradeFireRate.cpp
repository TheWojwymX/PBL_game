//
// Created by Jacek on 21.05.2024.
//

#include "UpgradeFireRate.h"

void UpgradeFireRate::Init() {
    SetVertices(_buttonVertices);
    SetText(_text, 1);
    Button::Init();
}

void UpgradeFireRate::Update() {
    Button::Update();
}

void UpgradeFireRate::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void UpgradeFireRate::Onclick() {
    UPGRADEMANAGER.UpgradeTurretFireRate();
}

void UpgradeFireRate::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}