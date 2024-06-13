#include "DomeUpgradeMenu.h"

void DomeUpgradeMenu::Init() {
    _upgradeDomeHP.Init();

    Page::Init();
}

void DomeUpgradeMenu::Update() {
    if(_shouldRender){
        Page::Update();
        _upgradeDomeHP.Update();
    }
}