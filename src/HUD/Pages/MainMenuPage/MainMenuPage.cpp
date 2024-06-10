//
// Created by Jacek on 10.06.2024.
//

#include "MainMenuPage.h"

void MainMenuPage::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetVertices(_backgroundVertices);
    _shouldRender = true;
    _startButton->Init();
    _settingsButton->Init();
    _creditsButton->Init();
    _quitButton->Init();
    Page::Init();
}

void MainMenuPage::Update() {
    if(_shouldRender){
        Page::Update();
        _startButton->Update();
        _settingsButton->Update();
        _creditsButton->Update();
        _quitButton->Update();
    }
}

void MainMenuPage::SetVertices(const array<float, 32> &vertices) {
    Page::SetVertices(vertices);
}