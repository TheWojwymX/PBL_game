//
// Created by Jacek on 09.05.2024.
//

#include "PageManager.h"
#include "Managers/ComponentsManager.h"
#include "Managers/GameManager.h"


PageManager &PageManager::getInstance() {
    static PageManager instance;
    return instance;
}

void PageManager::Init() {
    _pauseMenuPage->Init();
    _pages.push_back(_pauseMenuPage);
}

void PageManager::Update() {

    CheckInputs();

    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    _pauseMenuPage->Update();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

}

void PageManager::CheckInputs(){
    if(INPUT.IsKeyPressed(GLFW_KEY_ESCAPE)){
        _pauseMenuPage->_shouldRender = !_pauseMenuPage->_shouldRender;
        if(_pauseMenuPage->_shouldRender){
            EnableMouse();
            CloseAllOtherPages(_pauseMenuPage);
            INPUT.SetMouseFixedPos(0,0);
            GAMEMANAGER._paused = true;
        }
        else{
            DisableMouse();
            GAMEMANAGER._paused = false;
        }
    }
}

void PageManager::CloseAllOtherPages(const shared_ptr<Page>& pageException){
    for(const std::shared_ptr<Page>& page : _pages)
    {
        if(pageException == page){
            continue;
        }
        else{
            page->_shouldRender = false;
        }
    }
}

void PageManager::EnableMouse(){
    INPUT.SetCursorMode(true);
    GAMEMANAGER._editMode = true;
}

void PageManager::DisableMouse(){
    INPUT.SetCursorMode(false);
    GAMEMANAGER._editMode = false;
}
