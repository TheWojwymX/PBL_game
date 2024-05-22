//
// Created by Jacek on 21.05.2024.
//

#include "UpgradeSpeed.h"
#include "Managers/NodesManager.h"

void UpgradeSpeed::Init() {
    SetVertices(_buttonVertices);
    SetText(_text);
    Button::Init();
}

void UpgradeSpeed::Update() {
    Button::Update();
}

void UpgradeSpeed::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void UpgradeSpeed::Onclick() {
    NODESMANAGER.getNodeByName("player")->GetComponent<PlayerController>()->UpgradeSpeed();
}

void UpgradeSpeed::SetText(std::string text) {
    Button::SetText(text);
}