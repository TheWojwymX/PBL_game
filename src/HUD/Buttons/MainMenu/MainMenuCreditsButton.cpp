//
// Created by Jacek on 10.06.2024.
//

#include "MainMenuCreditsButton.h"

void MainMenuCreditsButton::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetHoverImagePath(_hoverBackgroundImagePath);
    SetClickedImagePath(_clickedBackgroundImagePath);
    SetVertices(_buttonVertices);
    SetText(_text, 1);
    Button::Init();
}

void MainMenuCreditsButton::Update() {
    Button::Update();
}

void MainMenuCreditsButton::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void MainMenuCreditsButton::Onclick() {
    std::cout << "test" << std::endl;
}

void MainMenuCreditsButton::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}