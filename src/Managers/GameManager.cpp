//
// Created by Jacek on 10.04.2024.
//

#include "GameManager.h"

GameManager &GameManager::getInstance() {
    static GameManager instance;
    return instance;
}
<<<<<<< Updated upstream
=======

void GameManager::pressToSkipPhase()
{
    if(INPUT.IsKeyPressed(77))
    {
        currentTime = 30.1;
    }
}

void GameManager::Update()
{
    currentTime += TIME.GetDeltaTime();
    pressToSkipPhase()

    if (currentTime >= phaseTime)
    {
        currentTime = 0.0f;
        currentPhase = (currentPhase + 1) % 3;
        InitPhase();
    }
}

void GameManager::InitPhase()
{
    if(currentPhase == 0) // kopanie
    {
        roundNumber++;
        //std::cout << "kopiemy";
    }
    else if (currentPhase == 1) // stawianie
    {
        //std::cout << "stawiamy";
    }
    else if (currentPhase == 2) // obrona
    {
        //std::cout << "bronimy";

        // For now only 3 rounds are added
        if (roundNumber > 3)
        {
            roundNumber = 3;
        }

        ENEMIESMANAGER.SpawnEnemiesForRound(roundNumber);
    }
}


void GameManager::Init() {
    _window = glfwCreateWindow(_screenWidth, _screenHeight, "SandBOX", NULL, NULL);
}
>>>>>>> Stashed changes
