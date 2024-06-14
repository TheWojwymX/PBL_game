#include "MainMenuCreditsButton.h"

void MainMenuCreditsButton::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetHoverImagePath(_hoverBackgroundImagePath);
    SetClickedImagePath(_clickedBackgroundImagePath);
    SetButtonPosition(glm::vec2(0,-5));
    SetText(_text, 1);
    Button::Init();
}

void MainMenuCreditsButton::Update() {
    Button::Update();
}

void MainMenuCreditsButton::Onclick() {
    std::cout << "test" << std::endl;
}

void MainMenuCreditsButton::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}