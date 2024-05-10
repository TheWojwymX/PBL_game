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

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    _pauseMenuPage.Update();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);


}
