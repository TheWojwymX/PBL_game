//
// Created by Jacek on 12.05.2024.
//

#include "SettingsButton.h"

void SettingsButton::Init() {
    SetVertices(_buttonVertices);
    SetText(_text);
    Button::Init();
}

void SettingsButton::Update() {
    Button::Update();
}

void SettingsButton::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void SettingsButton::Onclick() {
    std::cout << "Kliknieto opcje.";
    Button::Onclick();
}

void SettingsButton::SetText(std::string text) {
    Button::SetText(text);
}