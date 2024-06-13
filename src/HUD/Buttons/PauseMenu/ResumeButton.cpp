#include "ResumeButton.h"
#include "HUD/PageManager.h"
#include "Managers/GameManager.h"

void ResumeButton::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetHoverImagePath(_hoverBackgroundImagePath);
    SetClickedImagePath(_clickedBackgroundImagePath);
    SetText(_text, 1);
    Button::Init();
}

void ResumeButton::Update() {
    Button::Update();
}

void ResumeButton::Onclick() {
    std::cout << "Kliknieto wznowienie.";

    PAGEMANAGER._pauseMenuPage->_shouldRender = false;
    GAMEMANAGER.Unpause();

    Button::Onclick();
}

void ResumeButton::SetText(std::string text, float textSize) {
    Button::SetText(text, 1);
}
