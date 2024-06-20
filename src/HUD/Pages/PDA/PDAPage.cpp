#include "PDAPage.h"
#include "HUD/PageManager.h"

void PDAPage::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetCorners(CoordsConverter::ConvertCoords(glm::vec2(462, 770)), CoordsConverter::ConvertCoords(glm::vec2(1457, 309)));
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