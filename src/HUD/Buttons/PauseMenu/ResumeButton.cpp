//
// Created by Jacek on 09.05.2024.
//

#include "ResumeButton.h"
#include "HUD/PageManager.h"
#include "Managers/GameManager.h"

void ResumeButton::Init() {
    SetVertices(_buttonVertices);
    SetText(_text);
    Button::Init();
}

void ResumeButton::Update() {
    Button::Update();
}

void ResumeButton::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void ResumeButton::Onclick() {
    std::cout << "Kliknieto wznowienie.";

    PAGEMANAGER._pauseMenuPage->_shouldRender = false;
    PAGEMANAGER.DisableMouse();
    GAMEMANAGER._paused = false;

    Button::Onclick();
}

void ResumeButton::SetText(std::string text) {
    Button::SetText(text);
}