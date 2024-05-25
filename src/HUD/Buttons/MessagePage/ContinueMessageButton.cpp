//
// Created by Jacek on 25.05.2024.
//

#include "ContinueMessageButton.h"
#include "Managers/TutorialManager.h"
#include "HUD/PageManager.h"

void ContinueMessageButton::Init() {
    SetVertices(_buttonVertices);
    SetText(_text, 1);
    Button::Init();
}

void ContinueMessageButton::Update() {
    Button::Update();
}

void ContinueMessageButton::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void ContinueMessageButton::Onclick() {
    PAGEMANAGER.HideMessagePage();
    Button::Onclick();
}

void ContinueMessageButton::SetText(std::string text, float textSize) {
    Button::SetText(text, 0.6);
}