//
// Created by Jacek on 22.05.2024.
//

#include "JetpackRefuel.h"

void JetpackRefuel::Init() {
    SetVertices(_buttonVertices);
    SetText(_text);
    Button::Init();
}

void JetpackRefuel::Update() {
    Button::Update();
}

void JetpackRefuel::SetVertices(const array<float, 32> &vertices) {
    Button::SetVertices(vertices);
}

void JetpackRefuel::Onclick() {
    std::cout << "kliknieto na guzik refuela" << std::endl;
}

void JetpackRefuel::SetText(std::string text) {
    Button::SetText(text);
}