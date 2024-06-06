#pragma once

#include "GameManager.h"
#include "../Enemies/EnemiesManager.h"
#include "../Managers/TutorialManager.h"
GameManager &GameManager::getInstance() {
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
    _plastic += amount;
}

void GameManager::AddPlastic(int amount)
{
    _metal += amount;
}

void GameManager::Update()
{
    if(TUTORIALMANAGER._isTutorialEnded){
        currentTime += TIME.GetDeltaTime();
        pressToSkipPhase();

        if (currentTime >= phaseTime)
        {
            currentTime = 0.0f;
            currentPhase = (currentPhase + 1) % 3;
            InitPhase();
        }
    }
}

void GameManager::InitPhase()
{
    if(currentPhase == 0) // kopanie
    {
        roundNumber++;
        ENEMIESMANAGER.SetSymbolsForWave();
        //std::cout << "kopiemy";
    }
    else if (currentPhase == 1) // stawianie
    {
        //std::cout << "stawiamy";
    }
    else if (currentPhase == 2) // obrona
    {
        //std::cout << "bronimy";

        ENEMIESMANAGER.SpawnEnemiesForRound(roundNumber);
    }
}


void GameManager::Init() {
    _window = glfwCreateWindow(_screenWidth, _screenHeight, "SandBOX", glfwGetPrimaryMonitor(), NULL);
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
    EnableMouse();
}

void GameManager::Unpause() {
    _paused = false;
    DisableMouse();
}
