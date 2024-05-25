//
// Created by Jacek on 21.05.2024.
//

#include "UpgradeRadius.h"
#include "Managers/NodesManager.h"


void UpgradeRadius::Init() {
    SetVertices(_buttonVertices);
    SetText(_text, 1);
    Button::Init();
}

void UpgradeRadius::Update() {
    Button::Update();
}

void UpgradeRadius::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void UpgradeRadius::Onclick() {
    NODESMANAGER.getNodeByName("player")->GetComponent<PlayerController>()->UpgradeRadius();
}

void UpgradeRadius::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}