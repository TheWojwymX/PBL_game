//
// Created by Jacek on 21.05.2024.
//

#include "UpgradeReach.h"
#include "Managers/NodesManager.h"

void UpgradeReach::Init() {
    SetVertices(_buttonVertices);
    SetText(_text, 1);
    Button::Init();
}

void UpgradeReach::Update() {
    Button::Update();
}

void UpgradeReach::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void UpgradeReach::Onclick() {
    NODESMANAGER.getNodeByName("player")->GetComponent<PlayerController>()->UpgradeReach();
}

void UpgradeReach::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}