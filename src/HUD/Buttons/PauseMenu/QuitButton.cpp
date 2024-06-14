#include "QuitButton.h"
#include "Managers/GameManager.h"

void QuitButton::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetHoverImagePath(_hoverBackgroundImagePath);
    SetClickedImagePath(_clickedBackgroundImagePath);
    SetButtonPosition(glm::vec2(0,-10));
    SetText(_text, 1);
    Button::Init();
}

void QuitButton::Update() {
    Button::Update();
}

void QuitButton::Onclick() {
    std::cout << "Kliknieto wyjscie.";
    glfwSetWindowShouldClose(GAMEMANAGER._window, 1);
    Button::Onclick();
}

void QuitButton::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}
