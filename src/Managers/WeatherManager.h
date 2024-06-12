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

    glm::vec3 getWindDirection(){return windDirection;}
    float getWindStrength(){return windStrength;}

private:
    glm::vec3 windDirection;
    float windStrength;

    DrydenWind windModel;
};