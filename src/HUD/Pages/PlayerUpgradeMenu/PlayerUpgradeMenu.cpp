#include "PlayerUpgradeMenu.h"

void PlayerUpgradeMenu::Init() {
    Page::Init();
}

void PlayerUpgradeMenu::Update() {
    if(_shouldRender){
        Page::Update();
    }
}