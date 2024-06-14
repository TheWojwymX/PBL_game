#include "UpgradeRadius.h"
#include "Managers/NodesManager.h"


void UpgradeRadius::Init() {
    SetButtonPosition(glm::vec2(0,0));
    SetText(_text, 1);
    Button::Init();
}

void UpgradeRadius::Update() {
    Button::Update();
}

void UpgradeRadius::Onclick() {
    NODESMANAGER.getNodeByName("player")->GetComponent<PlayerController>()->UpgradeRadius();
}

void UpgradeRadius::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}