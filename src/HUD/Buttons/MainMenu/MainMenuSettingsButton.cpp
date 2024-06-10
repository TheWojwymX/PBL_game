//
// Created by Jacek on 10.06.2024.
//

#include "MainMenuSettingsButton.h"

void MainMenuSettingsButton::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetHoverImagePath(_hoverBackgroundImagePath);
    SetClickedImagePath(_clickedBackgroundImagePath);
    SetVertices(_buttonVertices);
    SetText(_text, 1);
    Button::Init();
}

void MainMenuSettingsButton::Update() {
    Button::Update();
}

void MainMenuSettingsButton::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void MainMenuSettingsButton::Onclick() {
    std::cout << "test" << std::endl;
}

void MainMenuSettingsButton::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}