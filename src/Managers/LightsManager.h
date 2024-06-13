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

    void Update();
    void InitLights();
    void AddGlowstick();
    void UpdateGlowsticks();

    bool isSpotActive = false;
    int glowstickCount = 0;

    float flashlightConstant = 10.0f;
    float flashlightLinear = 0.233f;
    float flashlightQuadratic = 0.06f;

    float glowstickConstant = 5.0f;
    float glowstickLinear = 2.049f;
    float glowstickQuadratic = 0.737f;
    /*
    float glowstickConstantNoFlash = 2.0f;   // Increased to reduce close-range intensity
    float glowstickLinearNoFlash = 0.7f;     // Increased to manage mid-range attenuation
    float glowstickQuadraticNoFlash = 0.1f;  // Slightly increased to smooth out the falloff
    */
    float glowstickConstantNoFlash = 3.0f;   // Maintained to control close-range intensity
    float glowstickLinearNoFlash = 0.4f;     // Decreased for more gradual mid-range attenuation
    float glowstickQuadraticNoFlash = 0.05f; // Decreased to allow light to reach further
private:
    void UpdateShaders();

    std::shared_ptr<Shader> instancedSandShader;
    std::shared_ptr<Shader> instancedMetalShader;
    std::shared_ptr<Shader> instancedPlasticShader;
    std::shared_ptr<Shader> modelShader;
    std::shared_ptr<Shader> shovelShader;
    std::shared_ptr<Shader> glowstickShader;

    glm::vec3 flashlightCurrentPosition = glm::vec3(0.0f);
    glm::vec3 flashlightCurrentDirection = glm::vec3(0.0f);
    glm::vec3 flashlightColor = glm::vec3(1.0f);
    float flashlightCutOff = 20.5f;
    float flashlightOuterCutOff = 24.5f;

    glm::vec3 glowstickColor = glm::vec3(1.0f);
    std::vector<glm::vec3> glowstickColors;

    int maxGlowsticks = 10;
};