#include "PauseMenuPage.h"

void PauseMenuPage::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    _logo.Init(_logoImagePath, glm::vec2(0,0),0, true, false);

    Page::Init();
}

void PauseMenuPage::Update() {
    if(_shouldRender){
        Page::Update();
        _logo.Render();
    }
}