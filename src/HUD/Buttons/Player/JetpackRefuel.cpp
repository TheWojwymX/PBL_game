#include "JetpackRefuel.h"

void JetpackRefuel::Init() {
    SetButtonPosition(glm::vec2(0, 0));
    SetText(_text, 1);
    Button::Init();
}

void JetpackRefuel::Update() {
    Button::Update();
}

void JetpackRefuel::Onclick() {
    std::cout << "kliknieto na guzik refuela" << std::endl;
}

void JetpackRefuel::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}