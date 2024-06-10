//
// Created by Jacek on 12.05.2024.
//

#include "QuitButton.h"
#include "Managers/GameManager.h"

void QuitButton::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetHoverImagePath(_hoverBackgroundImagePath);
    SetClickedImagePath(_clickedBackgroundImagePath);
    SetVertices(_buttonVertices);
    SetText(_text, 1);
    Button::Init();
}

void QuitButton::Update() {
    Button::Update();
}

void QuitButton::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void QuitButton::Onclick() {
    std::cout << "Kliknieto wyjscie.";
    glfwSetWindowShouldClose(GAMEMANAGER._window, 1);
    Button::Onclick();
}

void QuitButton::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}
