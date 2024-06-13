#include "PlayerUpgradeMenu.h"

void PlayerUpgradeMenu::Init() {
    _upgradeReachButton.Init();
    _upgradeRadiusButton.Init();
    _upgradeSpeedButton.Init();
    _jetpackRefuelButton.Init();

    Page::Init();
}

void PlayerUpgradeMenu::Update() {
    if(_shouldRender){
        Page::Update();
        _upgradeReachButton.Update();
        _upgradeRadiusButton.Update();
        _upgradeSpeedButton.Update();
        _jetpackRefuelButton.Update();
    }
}