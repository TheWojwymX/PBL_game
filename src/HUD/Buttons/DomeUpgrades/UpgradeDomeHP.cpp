#include "UpgradeDomeHP.h"

void UpgradeDomeHP::Init() {
    SetButtonPosition(glm::vec2(0,0));
    SetText(_text, 1);
    Button::Init();
}

void UpgradeDomeHP::Update() {
    Button::Update();
}

void UpgradeDomeHP::Onclick() {
    UPGRADEMANAGER.UpgradeDomeHp();
}

void UpgradeDomeHP::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}