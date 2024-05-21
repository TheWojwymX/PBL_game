//
// Created by Jacek on 21.05.2024.
//

#include "TurretUpgradeMenu.h"

void TurretUpgradeMenu::Init() {

    SetVertices(_backgroundVertices);

    _upgradeDamage.Init();
    _upgradeFireRate.Init();

    Page::Init();
}

void TurretUpgradeMenu::Update() {
    if(_shouldRender){
        Page::Update();
        _upgradeDamage.Update();
        _upgradeFireRate.Update();
    }
}

void TurretUpgradeMenu::SetVertices(const array<float, 32> &vertices) {
    Page::SetVertices(vertices);
}
