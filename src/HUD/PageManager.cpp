//
// Created by Jacek on 09.05.2024.
//

#include "PageManager.h"


PageManager &PageManager::getInstance() {
    static PageManager instance;
    return instance;
}

void PageManager::Init() {
    _pauseMenuPage.Init();
}

void PageManager::Update() {
    _pauseMenuPage.Update();
}
