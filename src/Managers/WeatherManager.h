#pragma once

#define WEATHERMANAGER WeatherManager::getInstance()

#include "ResourceManager.h"
#include "NodesManager.h"
#include "ComponentsManager.h"
#include "WindSimulation.h"

class WeatherManager{
public:
    static WeatherManager &getInstance();
    ~WeatherManager() = default;
    WeatherManager() = default;
    WeatherManager(const WeatherManager &) = delete;
    WeatherManager &operator=(const WeatherManager &) = delete;

    void Init();
    void Update();

    void SetupAmbientParticles();
    void SetupRainParticles();
    void UpdateSunPosition();

    glm::vec3 getWindDirection(){return windDirection;}
    float getWindStrength(){return windStrength;}

private:
    glm::vec3 windDirection;
    float windStrength;

    DrydenWind windModel;

    bool isRaining = false;
    float rainProbability = 0.1f; //100%
    float rainCheckInterval = 5.0f;
    float rainDuration = 40.0f;
    float timeSinceLastCheck = 0.0f;
    float rainTimeLeft = 0.0f;
    int spawnedParticles = 0;
    float particleSpawnInterval = 0.016f;
    float timeSinceLastParticleSpawn = 0.0f;
    float timer = 0.0f;
    bool increasing = false;

    float radius = 50.0f;
    float angle = 45.0f;
    float speed = 0.3f;
    int direction = 1;
};