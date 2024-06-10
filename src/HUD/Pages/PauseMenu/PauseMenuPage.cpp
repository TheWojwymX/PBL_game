//
// Created by Jacek on 09.05.2024.
//

#include "PauseMenuPage.h"

void PauseMenuPage::Init() {

    SetBackgroundImagePath(_backgroundImagePath);
    SetVertices(_backgroundVertices);
    _resumeButton.Init();
    _quitButton.Init();
    _settingsButton.Init();
    _logo.Init(_logoImagePath, _logoVertices, true, false);

    Page::Init();
}

void PauseMenuPage::Update() {
    if(_shouldRender){
        Page::Update();
        _resumeButton.Update();
        _quitButton.Update();
        _settingsButton.Update();
        _logo.UpdateImage();
    }
}

void PauseMenuPage::SetVertices(const array<float, 32> &vertices) {
    Page::SetVertices(vertices);
}
