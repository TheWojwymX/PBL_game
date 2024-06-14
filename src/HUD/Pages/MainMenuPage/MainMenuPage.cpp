#include "MainMenuPage.h"

void MainMenuPage::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetCorners(glm::vec2(-50, -50), glm::vec2(50, 50));
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