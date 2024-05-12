//
// Created by Jacek on 09.05.2024.
//

#include "PauseMenuPage.h"

void PauseMenuPage::Init() {

    SetVertices(_backgroundVertices);

    _resumeButton.Init();
    _quitButton.Init();
    _settingsButton.Init();

    Page::Init();
}

void PauseMenuPage::Update() {
    if(_shouldRender){
        Page::Update();
        _resumeButton.Update();
        _quitButton.Update();
        _settingsButton.Update();
    }
}

void PauseMenuPage::SetVertices(const array<float, 32> &vertices) {
    Page::SetVertices(vertices);
}
