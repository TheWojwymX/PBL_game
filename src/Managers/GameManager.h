#define GAMEMANAGER GameManager::getInstance()

#ifndef OPENGLGP_GAMEMANAGER_H
#define OPENGLGP_GAMEMANAGER_H

#include "Core/Node.h"

class GameManager {

public:
    bool _isFullscreen = false;

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
    void AddMetal(int amount);
    void AddPlastic(int amount);

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

    glm::vec3 _windDirection{0.0f};
    float _windStrength = 0.0f;

    void EnableMouse();
    void DisableMouse();

    void Pause();
    void Unpause();

    int _metal = 50;
    int _plastic = 50;
};


#endif //OPENGLGP_GAMEMANAGER_H
