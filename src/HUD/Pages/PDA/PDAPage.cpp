#include "PDAPage.h"
#include "HUD/PageManager.h"

void PDAPage::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetCorners(glm::vec2(-25, -25), glm::vec2(25, 25));
    Page::Init();
}

void PDAPage::Update() {
    if(_shouldRender){
        Page::Update();
    }
}

void PDAPage::DisplayPDAPage(){
    PAGEMANAGER._isInPage = true;
    _shouldRender = true;
}

void PDAPage::HidePDAPage(){
    PAGEMANAGER._isInPage = false;
    _shouldRender = false;
}