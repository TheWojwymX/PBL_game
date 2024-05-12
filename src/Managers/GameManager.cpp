//
// Created by Jacek on 10.04.2024.
//

#include "GameManager.h"

GameManager &GameManager::getInstance() {
    static GameManager instance;
    return instance;
}

void GameManager::Init() {
    _window = glfwCreateWindow(_screenWidth, _screenHeight, "SandBOX", NULL, NULL);
}
