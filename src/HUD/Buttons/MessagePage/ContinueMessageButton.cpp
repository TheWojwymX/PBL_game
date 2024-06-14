#include "ContinueMessageButton.h"
#include "Managers/TutorialManager.h"
#include "HUD/PageManager.h"

void ContinueMessageButton::Init() {
    SetButtonPosition(glm::vec2(0,0));
    SetText(_text, 1);
    Button::Init();
}

void ContinueMessageButton::Update() {
    Button::Update();
}

void ContinueMessageButton::Onclick() {
    PAGEMANAGER.HideMessagePage();
    Button::Onclick();
}

void ContinueMessageButton::SetText(std::string text, float textSize) {
    Button::SetText(text, 0.6);
}