//
// Created by Jacek on 21.05.2024.
//

#include "DomeUpgradeMenu.h"

void DomeUpgradeMenu::Init() {

    SetVertices(_backgroundVertices);

    _upgradeDomeHP.Init();

    Page::Init();
}

void DomeUpgradeMenu::Update() {
    if(_shouldRender){
        Page::Update();
        _upgradeDomeHP.Update();
    }
}

void DomeUpgradeMenu::SetVertices(const array<float, 32> &vertices) {
    Page::SetVertices(vertices);
}