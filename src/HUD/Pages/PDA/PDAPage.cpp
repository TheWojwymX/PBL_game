//
// Created by Jacek on 08.06.2024.
//

#include "PDAPage.h"
#include "HUD/PageManager.h"

void PDAPage::Init() {
    SetBackgroundImagePath(_PDABackgroundPath);
    SetVertices(_backgroundVertices);
    Page::Init();
}

void PDAPage::Update() {
    if(_shouldRender){
        Page::Update();
    }
}

void PDAPage::SetVertices(const array<float, 32> &vertices) {
    Page::SetVertices(vertices);
}

void PDAPage::DisplayPDAPage(){
    PAGEMANAGER._isInPage = true;
    _shouldRender = true;
}

void PDAPage::HidePDAPage(){
    PAGEMANAGER._isInPage = false;
    _shouldRender = false;
}