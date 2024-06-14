#include "DomeUpgradeMenu.h"

void DomeUpgradeMenu::Init() {
    Page::Init();
}

void DomeUpgradeMenu::Update() {
    if(_shouldRender){
        Page::Update();
    }
}