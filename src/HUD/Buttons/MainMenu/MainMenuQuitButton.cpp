#include "MainMenuQuitButton.h"
#include "Managers/GameManager.h"

void MainMenuQuitButton::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetHoverImagePath(_hoverBackgroundImagePath);
    SetClickedImagePath(_clickedBackgroundImagePath);
    SetButtonPosition(glm::vec2(0, 0));
    SetText(_text, 1);
    Button::Init();
}

void MainMenuQuitButton::Update() {
    Button::Update();
}

void MainMenuQuitButton::Onclick() {
    glfwSetWindowShouldClose(GAMEMANAGER._window, 1);
}

void MainMenuQuitButton::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}