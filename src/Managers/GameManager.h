//
// Created by Jacek on 10.04.2024.
//

#define GAMEMANAGER GameManager::getInstance()

#ifndef OPENGLGP_GAMEMANAGER_H
#define OPENGLGP_GAMEMANAGER_H

#include "Core/Node.h"

class GameManager {

public:
    int _screenWidth = 1280;
    int _screenHeight = 720;
    GLFWwindow* _window;

    void Init();

    static GameManager &getInstance();
    ~GameManager() = default;
    GameManager() = default;
    GameManager(const GameManager &) = delete;
    GameManager &operator=(const GameManager &) = delete;
    void Update();
    void InitPhase();
    void pressToSkipPhase();

    std::shared_ptr<Node> root;
    int _mapSizeX = 200;
    int _mapSizeZ = 200;
    int _domeRadius = 13.5;
    glm::vec2 _domePosition = glm::vec2(49.5, 49.5);
    float _mineEntranceRadius = 3.0f;
    float _groundLevel = 300.2;
    int roundNumber = -1;
    int currentPhase = 0; // 0 - kopanie, 1 - ustawianie, 2 - obrona
    float currentTime = 0.0f;
    float phaseTime = 30.0f;

    bool _editMode = true;

    bool _paused = false;

    int _money = 50;

    void EnableMouse();
    void DisableMouse();

    void Pause();
    void Unpause();
};


#endif //OPENGLGP_GAMEMANAGER_H
