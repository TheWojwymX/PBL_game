#include "MainMenuStartButton.h"
#include "Managers/GameManager.h"

void MainMenuStartButton::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetHoverImagePath(_hoverBackgroundImagePath);
    SetClickedImagePath(_clickedBackgroundImagePath);
    SetButtonPosition(glm::vec2(0,15));
    SetText(_text, 1);
    Button::Init();
}

void MainMenuStartButton::Update() {
    Button::Update();
}

void MainMenuStartButton::Onclick() {
    GAMEMANAGER._isInMainMenu = false;
}

void MainMenuStartButton::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}