#pragma once

#include "GameManager.h"
#include "../Enemies/EnemiesManager.h"
#include "../HUD/PageManager.h"
#include "../Managers/TutorialManager.h"
#include "Managers/NodesManager.h"

GameManager &GameManager::GetInstance() {
    static GameManager instance;
    return instance;
}

void GameManager::pressToSkipPhase()
{
    if(INPUT.IsKeyPressed(80))
    {
        currentTime = 31;
    }
}

void GameManager::AddMetal(int amount)
{
    HUD._isMetalInAnim = true;
    _metal += amount;
}

void GameManager::AddPlastic(int amount)
{
    HUD._isPlasticInAnim = true;
    _plastic += amount;
}

bool GameManager::HasMaterials(glm::ivec2 mat) {
    return mat.x <= _plastic && mat.y <= _metal;
}

void GameManager::RemoveMaterials(glm::ivec2 mat)
{
    if(mat.x != 0){
        _plastic -= mat.x;
        HUD._isPlasticInAnim = true;
    }

    if(mat.y != 0){
        _metal -= mat.y;
        HUD._isMetalInAnim = true;
    }
}

void GameManager::StartGame()
{
    _isInMainMenu = false;
    GAMEMANAGER._editMode = false;
    INPUT.SetCursorMode(false);
}

void GameManager::Evacuate()
{
    std::cout << "Evacuation in GAMEMANAGER" << std::endl;
}

bool GameManager::IsUnderground()
{
    if(_playerNode == nullptr) _playerNode = NODESMANAGER.getNodeByName("player");

    float playerY = _playerNode->GetTransform()->GetPosition().y;

    return playerY < _groundLevel;
}

void GameManager::Update()
{
    if(TUTORIALMANAGER._isFreePlay){
        currentTime += TIME.GetDeltaTime();
        pressToSkipPhase();

        if (currentTime >= phaseTime)
        {
            currentTime = 0.0f;
            currentPhase = (currentPhase + 1) % 2;
            InitPhase();
        }
    }
}

void GameManager::InitPhase()
{
    if(currentPhase == 0) // kopanie
    {
        roundNumber++;
    }
    else // obrona
    {
        ENEMIESMANAGER.SpawnEnemiesForRound(roundNumber);
    }
}


void GameManager::Init() {
    if (_isFullscreen) {
        _screenWidth = 1920;
        _screenHeight = 1080;
        _window = glfwCreateWindow(_screenWidth, _screenHeight, "SandBOX", glfwGetPrimaryMonitor(), NULL);
    }
    else {
        _window = glfwCreateWindow(_screenWidth, _screenHeight, "SandBOX", NULL, NULL);
    }

    if (!_window) {
        std::cerr << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return;
    }

    // Make the OpenGL context current for the new window
    glfwMakeContextCurrent(_window);

    // Retrieve the actual dimensions of the framebuffer
    int actualWidth, actualHeight;
    glfwGetFramebufferSize(_window, &actualWidth, &actualHeight);

    // Update _screenWidth and _screenHeight with the actual dimensions
    _screenWidth = actualWidth;
    _screenHeight = actualHeight;
}

void GameManager::EnableMouse() {
    INPUT.SetCursorMode(true);
    _editMode = true;
}

void GameManager::DisableMouse() {
    INPUT.SetCursorMode(false);
    _editMode = false;
}

void GameManager::Pause() {
    _paused = true;
    PAGEMANAGER._pauseMenuPage->_shouldRender = true;
    HUD.DisableHUD();
    EnableMouse();
}

void GameManager::Unpause() {
    _paused = false;
    PAGEMANAGER._pauseMenuPage->_shouldRender = false;
    HUD.EnableHUD();
    DisableMouse();
}