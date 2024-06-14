#include "UpgradeSpeed.h"
#include "Managers/NodesManager.h"

void UpgradeSpeed::Init() {
    SetButtonPosition(glm::vec2(0,3));
    SetText(_text, 1);
    Button::Init();
}

void UpgradeSpeed::Update() {
    Button::Update();
}

void UpgradeSpeed::Onclick() {
    NODESMANAGER.getNodeByName("player")->GetComponent<PlayerController>()->UpgradeSpeed();
}

void UpgradeSpeed::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}