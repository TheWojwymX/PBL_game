#include "TurretUpgradeMenu.h"

void TurretUpgradeMenu::Init() {
    Page::Init();
}

void TurretUpgradeMenu::Update() {
    if(_shouldRender){
        Page::Update();
    }
}