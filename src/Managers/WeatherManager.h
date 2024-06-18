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
    glm::vec3 getSkyColor();
    glm::vec3 getDirColor();

    glm::vec3 getWindDirection(){return windDirection;}
    float getWindStrength(){return windStrength;}

private:
    glm::vec3 windDirection;
    float windStrength;

    DrydenWind windModel;

    bool isRaining = false;
    float rainProbability = 0.2f; //100%
    float rainCheckInterval = 3.0f;
    float rainDuration = 80.0f;
    float timeSinceLastCheck = 0.0f;
    float rainTimeLeft = 0.0f;
    int spawnedParticles = 0;
    float particleSpawnInterval = 0.016f;
    float timeSinceLastParticleSpawn = 0.0f;
    float timer = 0.0f;
    bool increasing = false;

    float radius = 50.0f;
    float angle = 44.5f;
    float speed = 0.03f;
    int direction = 1;
    int counter = 0;
    int angleDirection = 1;

    glm::vec3 daySkyColor;
    glm::vec3 sunsetSkyColor;
    glm::vec3 nightSkyColor;
    glm::vec3 rainDaySkyColor;
    glm::vec3 rainSunsetSkyColor;
    glm::vec3 rainNightSkyColor;

    glm::vec3 dayColor;
    glm::vec3 sunsetColor;
    glm::vec3 nightColor;
    glm::vec3 rainDayColor;
    glm::vec3 rainSunsetColor;
    glm::vec3 rainNightColor;

    bool rainyDay = false;
    bool thunder = false;
    float thunderTimeLeft = 0.0f;
    float thunderDuration = 0.2f;
    float thunderProbability = 0.6f;

    float shadowStrength = 1.0f;
    float dirAngle = 44.5f;

    float stepInterval = 0.2f;
    float timeAccumulator = 0.0f;
};