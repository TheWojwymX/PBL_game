#include "UpgradeReach.h"
#include "Managers/NodesManager.h"

void UpgradeReach::Init() {
    SetButtonPosition(glm::vec2(0,0));
    SetText(_text, 1);
    Button::Init();
}

void UpgradeReach::Update() {
    Button::Update();
}

void UpgradeReach::Onclick() {
    NODESMANAGER.getNodeByName("player")->GetComponent<PlayerController>()->UpgradeReach();
}

void UpgradeReach::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}