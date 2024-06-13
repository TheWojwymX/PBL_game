//
// Created by Jacek on 21.05.2024.
//

#include "UpgradeDomeHP.h"

void UpgradeDomeHP::Init() {
    SetVertices(_buttonVertices);
    SetText(_text, 1);
    Button::Init();
}

void UpgradeDomeHP::Update() {
    Button::Update();
}

void UpgradeDomeHP::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void UpgradeDomeHP::Onclick() {
    UPGRADEMANAGER.UpgradeDomeHp();
}

void UpgradeDomeHP::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}