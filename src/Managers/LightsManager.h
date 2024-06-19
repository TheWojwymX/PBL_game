#pragma once

#define LIGHTSMANAGER LightsManager::getInstance()

#include "ResourceManager.h"
#include "NodesManager.h"
#include "ComponentsManager.h"
#include "Component/Camera.h"

enum LightSource {GLOWSTICK, SHOT};

struct ActiveShot {
    glm::vec3 startPos;
    glm::vec3 endPos;
    glm::vec3 color;
    float elapsedTime;

    ActiveShot(const glm::vec3& start, const glm::vec3& end, const glm::vec3& col)
        : startPos(start), endPos(end), color(col), elapsedTime(0.0f) {
    }
};

struct VisibleLight {
    glm::vec3 pos;
    glm::vec3 color;
    float distance;
    LightSource sourceType;

    VisibleLight(const glm::vec3& position, const glm::vec3& col, float dist, LightSource lightSource)
        : pos(position), color(col), distance(dist), sourceType(lightSource) {
    }
};

class LightsManager{
public:
    static LightsManager &getInstance();
    ~LightsManager() = default;
    LightsManager() = default;
    LightsManager(const LightsManager &) = delete;
    LightsManager &operator=(const LightsManager &) = delete;

    void Update();
    void Reset();
    void InitLights();
    void AddGlowstick();
    void AddShot(const glm::vec3& startPos, const glm::vec3& endPos);
    void TurnOffGlowsticks();

    int glowstickCount = 0;

    float flashlightConstant = 10.0f;
    float flashlightLinear = 0.233f;
    float flashlightQuadratic = 0.06f;

    float glowstickConstant = 3.093f;
    float glowstickLinear = 0.632f;
    float glowstickQuadratic = 0.165f;

    float shotConstant = 1.05f;
    float shotLinear = 0.6f;
    float shotQuadratic = 1.5f;

    glm::vec3 dirColor{ 0.999f, 0.999f, 1.00f };
    glm::vec3 skyColor{ 0.6235f, 0.6235f, 0.8039f };
    float dirDirection[3] = { -0.5f, -0.5f, -0.5f };
    bool dirActive = true;

    glm::vec3 lightPos;
    glm::vec3 lightCenter;

    glm::vec3 lightPosShader;

    glm::vec3 shadowLightPos;
    glm::vec3 shadowLightCenter;
private:
    void UpdateShaders();

    std::shared_ptr<Shader> instancedSandShader;
    std::shared_ptr<Shader> instancedMetalShader;
    std::shared_ptr<Shader> instancedPlasticShader;
    std::shared_ptr<Shader> modelShader;
    std::shared_ptr<Shader> shovelShader;
    std::shared_ptr<Shader> glowstickShader;
    std::shared_ptr<Shader> outlineShader;
    std::shared_ptr<Shader> lightObjectShader;
    std::shared_ptr<Shader> cloudShader;
    std::shared_ptr<Shader> particleShader;

    glm::vec3 flashlightCurrentPosition = glm::vec3(0.0f);
    glm::vec3 flashlightCurrentDirection = glm::vec3(0.0f);
    glm::vec3 flashlightColor = glm::vec3(1.0f);
    float flashlightCutOff = 20.5f;
    float flashlightOuterCutOff = 24.5f;

    glm::vec3 glowstickColor = glm::vec3(1.0f);
    std::vector<glm::vec3> glowstickColors;

    int maxLights = 30;
    float maxDistance = 60;

    std::vector<std::shared_ptr<Node>> _glowsticksNodes;
    std::vector<VisibleLight> _visibleGlowsticks;
    int _oldAmountOfGlowsticks = 0;

    std::vector<ActiveShot> _activeShots;
    std::vector<VisibleLight> _visibleShots;
    std::vector<std::shared_ptr<Shader>> _shadersForPointLights;
    std::shared_ptr<Camera> _cameraRef;
    float _bulletSpeed = 4.0f;

    void UpdateActiveShots();
    void UpdateGlowsticks();
    void SortActiveShots();
    void UpdateLights();
    void InitShadersForPointLights();
    void SortVisibleLights(std::vector<VisibleLight>& lights);
    void UpdateShadersPointLights(const std::vector<VisibleLight>& pointLights);
    void UpdateShaderPointLight(std::shared_ptr<Shader> shader, string name, glm::vec3 pos, glm::vec3 color, LightSource lightSource);


    glm::vec3 GenerateRandomColor();
};