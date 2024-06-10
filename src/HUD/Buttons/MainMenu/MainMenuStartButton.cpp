//
// Created by Jacek on 10.06.2024.
//

#include "MainMenuStartButton.h"
#include "Managers/GameManager.h"

void MainMenuStartButton::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetHoverImagePath(_hoverBackgroundImagePath);
    SetClickedImagePath(_clickedBackgroundImagePath);
    SetVertices(_buttonVertices);
    SetText(_text, 1);
    Button::Init();
}

void MainMenuStartButton::Update() {
    Button::Update();
}

void MainMenuStartButton::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void MainMenuStartButton::Onclick() {
    GAMEMANAGER._isInMainMenu = false;
}

void MainMenuStartButton::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}