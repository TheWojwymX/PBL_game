#pragma once

#define GAMEMANAGER GameManager::GetInstance()

#include "Core/Node.h"
#include <vector>

enum Phase {DIG,DEFEND};

class GameManager {
public:
    // Public members
    bool _isFullscreen = false;
    int _screenWidth = 1920;
    int _screenHeight = 1080;
    GLFWwindow* _window;

    void Init();
    static GameManager& GetInstance();
    ~GameManager() = default;
    GameManager();
    GameManager(const GameManager&) = delete;
    GameManager& operator=(const GameManager&) = delete;

    void Update();
    void InitPhase();
    void pressToSkipPhase();
    void AddPlastic(int amount);
    bool HasPlastic(int amount);
    void RemovePlastic(int amount);
    void StartGame();
    void Evacuate();
    bool IsUnderground();
    float GetPhaseTime();

    Phase GetPhase() {return _currentPhase;}
    float GetCurrentTime() { return _currentTime; }
    int GetPlastic() { return _plastic; }
    int GetRoundNumber() { return _roundNumber; }

    std::shared_ptr<Node> root;
    int _mapSizeX = 100;
    int _mapSizeZ = 100;
    int _domeRadius = 13.5;
    glm::vec2 _domePosition = glm::vec2(49.5, 49.5);
    float _mineEntranceRadius = 3.0f;
    float _groundLevel = 299.5;

    bool _editMode = true;
    bool _paused = false;
    glm::vec3 _windDirection{0.0f};
    float _windStrength = 0.0f;

    void EnableMouse();
    void DisableMouse();

    void Pause();
    void Unpause();

    bool _isInMainMenu = true;

    glm::vec2 _domeStationPosition = glm::vec2(43.3, 49.5);
    float _domeStationRadius = 3;

    glm::vec2 _playerStationPosition = glm::vec2(55.7, 49.5);
    float _playerStationRadius = 3;

    void Reset();

    void PlayMenuMusic();

    void StopMenuMusic();

    void RestartGame();

    void LoseGame();

private:
    int _roundNumber = 0;
    Phase _currentPhase = Phase::DIG; 
    int _plastic = 5;
    float _currentTime = 0.0f;

    std::vector<std::pair<float, float>> _phaseTimes;
    std::shared_ptr<Node> _playerNode = nullptr;
};