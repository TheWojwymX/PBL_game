#include "MainMenuSettingsButton.h"

void MainMenuSettingsButton::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetHoverImagePath(_hoverBackgroundImagePath);
    SetClickedImagePath(_clickedBackgroundImagePath);
    SetButtonPosition(glm::vec2(0,0));
    SetText(_text, 1);
    Button::Init();
}

void MainMenuSettingsButton::Update() {
    Button::Update();
}

void MainMenuSettingsButton::Onclick() {
    std::cout << "test" << std::endl;
}

void MainMenuSettingsButton::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}