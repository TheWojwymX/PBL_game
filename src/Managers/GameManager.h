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
    void Update();
    void InitPhase();

    std::shared_ptr<Node> root;
    int _mapSizeX = 100;
    int _mapSizeZ = 100;
    int _domeRadius = 15;
    glm::vec2 _domePosition = glm::vec2(50, 50);
    float _groundLevel = 100.2;
    int roundNumber = -1;
    int currentPhase = 0; // 0 - kopanie, 1 - ustawianie, 2 - obrona
    float currentTime = 11.0f;
    float phaseTime = 10.0f;

};


#endif //OPENGLGP_GAMEMANAGER_H
