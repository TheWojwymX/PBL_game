//
// Created by Jacek on 10.04.2024.
//

#define GAMEMANAGER GameManager::getInstance()

#ifndef OPENGLGP_GAMEMANAGER_H
#define OPENGLGP_GAMEMANAGER_H

#include "Core/Node.h"

class GameManager {

public:

    static GameManager &getInstance();

    ~GameManager() = default;

    GameManager() = default;

    GameManager(const GameManager &) = delete;

    GameManager &operator=(const GameManager &) = delete;

    std::shared_ptr<Node> root;


};


#endif //OPENGLGP_GAMEMANAGER_H
