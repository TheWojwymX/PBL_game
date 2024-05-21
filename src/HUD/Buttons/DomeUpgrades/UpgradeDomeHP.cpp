//
// Created by Jacek on 21.05.2024.
//

#include "UpgradeDomeHP.h"

void UpgradeDomeHP::Init() {
    SetVertices(_buttonVertices);
    SetText(_text);
    Button::Init();
}

void UpgradeDomeHP::Update() {
    Button::Update();
}

void UpgradeDomeHP::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void UpgradeDomeHP::Onclick() {
    UPGRADEMANAGER.upgradeDomeHp();
}

void UpgradeDomeHP::SetText(std::string text) {
    Button::SetText(text);
}