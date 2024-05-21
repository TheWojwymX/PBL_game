//
// Created by Jacek on 09.05.2024.
//

#include "PlayerUpgradeMenu.h"

void PlayerUpgradeMenu::Init() {

    SetVertices(_backgroundVertices);

    _upgradeReachButton.Init();
    _upgradeRadiusButton.Init();
    _upgradeSpeedButton.Init();

    Page::Init();
}

void PlayerUpgradeMenu::Update() {
    if(_shouldRender){
        Page::Update();
        _upgradeReachButton.Update();
        _upgradeRadiusButton.Update();
        _upgradeSpeedButton.Update();
    }
}

void PlayerUpgradeMenu::SetVertices(const array<float, 32> &vertices) {
    Page::SetVertices(vertices);
}