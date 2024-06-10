//
// Created by Jacek on 10.06.2024.
//

#include "MainMenuQuitButton.h"
#include "Managers/GameManager.h"

void MainMenuQuitButton::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetHoverImagePath(_hoverBackgroundImagePath);
    SetClickedImagePath(_clickedBackgroundImagePath);
    SetVertices(_buttonVertices);
    SetText(_text, 1);
    Button::Init();
}

void MainMenuQuitButton::Update() {
    Button::Update();
}

void MainMenuQuitButton::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void MainMenuQuitButton::Onclick() {
    glfwSetWindowShouldClose(GAMEMANAGER._window, 1);
}

void MainMenuQuitButton::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}