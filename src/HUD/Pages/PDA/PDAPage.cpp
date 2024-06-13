#include "PDAPage.h"
#include "HUD/PageManager.h"

void PDAPage::Init() {
    SetBackgroundImagePath(_PDABackgroundPath);

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