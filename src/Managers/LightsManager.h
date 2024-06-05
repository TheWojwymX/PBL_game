#pragma once

#define LIGHTSMANAGER LightsManager::getInstance()

#include "ResourceManager.h"
#include "NodesManager.h"
#include "ComponentsManager.h"
#include "Component/Camera.h"

class LightsManager{
public:
    static LightsManager &getInstance();
    ~LightsManager() = default;
    LightsManager() = default;
    LightsManager(const LightsManager &) = delete;
    LightsManager &operator=(const LightsManager &) = delete;

    void UpdateLights();
    void InitLights();
    void AddGlowstick();
    void UpdateGlowsticks();

    bool isSpotActive = false;
    int glowstickCount = 0;
private:
    void UpdateShaders();

    std::shared_ptr<Shader> instancedSandShader;
    std::shared_ptr<Shader> instancedMetalShader;
    std::shared_ptr<Shader> modelShader;
    std::shared_ptr<Shader> shovelShader;

    glm::vec3 flashlightCurrentPosition = glm::vec3(0.0f);
    glm::vec3 flashlightCurrentDirection = glm::vec3(0.0f);
    glm::vec3 flashlightColor = glm::vec3(1.0f);
    float flashlightConstant = 2.0f;
    float flashlightLinear = 0.15f;
    float flashlightQuadratic = 0.06f;
    float flashlightCutOff = 20.5f;
    float flashlightOuterCutOff = 24.5f;

    glm::vec3 glowstickColor = glm::vec3(1.0f);
    std::vector<glm::vec3> glowstickColors;
    float glowstickConstant = 3.0f;
    float glowstickLinear = 3.0f;
    float glowstickQuadratic = 3.0f;
    int maxGlowsticks = 10;
};