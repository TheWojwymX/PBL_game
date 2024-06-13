#include "PauseMenuPage.h"

void PauseMenuPage::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    _resumeButton.Init();
    _quitButton.Init();
    _settingsButton.Init();
    _logo.Init(_logoImagePath, glm::vec3(0,0,0), true, false);

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