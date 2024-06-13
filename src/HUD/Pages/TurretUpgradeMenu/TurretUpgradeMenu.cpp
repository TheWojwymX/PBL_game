#include "TurretUpgradeMenu.h"

void TurretUpgradeMenu::Init() {
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