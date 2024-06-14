#include "SettingsButton.h"

void SettingsButton::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetHoverImagePath(_hoverBackgroundImagePath);
    SetClickedImagePath(_clickedBackgroundImagePath);
    SetButtonPosition(glm::vec2(0,-7));
    SetText(_text, 1);
    Button::Init();
}

void SettingsButton::Update() {
    Button::Update();
}

void SettingsButton::Onclick() {
    std::cout << "Kliknieto opcje.";
    Button::Onclick();
}

void SettingsButton::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}