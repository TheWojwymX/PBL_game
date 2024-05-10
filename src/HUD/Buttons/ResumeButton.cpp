//
// Created by Jacek on 09.05.2024.
//

#include "ResumeButton.h"

void ResumeButton::Init() {
    SetVertices(_buttonVertices);
    Button::Init();
}

void ResumeButton::Update() {
    Button::Update();
}

void ResumeButton::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void ResumeButton::Onclick() {
    std::cout << "no kliknieto xD";
    Button::Onclick();
}
