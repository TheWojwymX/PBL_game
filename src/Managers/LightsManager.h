#pragma once

#define LIGHTSMANAGER LightsManager::getInstance()

#include "ResourceManager.h"
#include "NodesManager.h"
#include "ComponentsManager.h"
#include "Component/Camera.h"
#include "BatchRandomGenerator.h"

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
    LightsManager();
    LightsManager(const LightsManager &) = delete;
    LightsManager &operator=(const LightsManager &) = delete;

    void Update();
    void Reset();
    void InitLights();
    void AddGlowstick();
    void AddShot(const glm::vec3& startPos, const glm::vec3& endPos);
    void TurnOffGlowsticks();
    void AssignNewGlowstickSettings(char level);
    void ChangeDisco();

    int _glowstickCount = 0;

    float flashlightConstant = 10.0f;
    float flashlightLinear = 0.233f;
    float flashlightQuadratic = 0.06f;
    glm::vec3 flashlightColor = glm::vec3(1.0f);

    float scale = 2.0f;

    float glowstickConstant = 3.093f * scale;
    float glowstickLinear = 0.632f * scale;
    float glowstickQuadratic = 0.165f * scale; // 0.20625f

    float glowstickConstant0 = 3.093f * scale;
    float glowstickLinear0 = 0.632f * scale;
    float glowstickQuadratic0 = 0.165f * scale; // 0.20625f

    float glowstickConstant1 = 3.093f * scale;
    float glowstickLinear1 = 0.632f * scale;
    float glowstickQuadratic1 = 0.155f * scale; // 0.19375f

    float glowstickConstant2 = 3.093f * scale;
    float glowstickLinear2 = 0.632f * scale;
    float glowstickQuadratic2 = 0.135f * scale; // 0.16875f

    float glowstickConstant3 = 3.093f * scale;
    float glowstickLinear3 = 0.632f * scale;
    float glowstickQuadratic3 = 0.105f * scale; // 0.13125f

    float glowstickConstant4 = 3.093f * scale;
    float glowstickLinear4 = 0.632f * scale;
    float glowstickQuadratic4 = 0.075f * scale; // 0.09375f

    float glowstickConstant5 = 3.093f * scale;
    float glowstickLinear5 = 0.632f * scale;
    float glowstickQuadratic5 = 0.045f * scale; // 0.05625f



    float shotConstant = 1.066f;
    float shotLinear = 1.438f;
    float shotQuadratic = 5.899f;

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

    BatchRandomGenerator _randomGen;
    std::queue<glm::vec3> _randomColors;

    std::shared_ptr<Shader> instancedSandShader;
    std::shared_ptr<Shader> modelShader;
    std::shared_ptr<Shader> shovelShader;
    std::shared_ptr<Shader> glowstickShader;
    std::shared_ptr<Shader> outlineShader;
    std::shared_ptr<Shader> lightObjectShader;
    std::shared_ptr<Shader> cloudShader;
    std::shared_ptr<Shader> particleShader;

    glm::vec3 flashlightCurrentPosition = glm::vec3(0.0f);
    glm::vec3 flashlightCurrentDirection = glm::vec3(0.0f);

    float flashlightCutOff = 20.5f;
    float flashlightOuterCutOff = 24.5f;

    glm::vec3 glowstickColor = glm::vec3(1.0f);
    std::vector<glm::vec3> _glowstickColors;

    int maxLights = 50;
    float maxDistance = 60;
    bool _isDisco = false;

    std::vector<std::shared_ptr<Node>> _glowsticksNodes;
    std::vector<VisibleLight> _visibleGlowsticks;

    std::vector<ActiveShot> _activeShots;
    std::vector<VisibleLight> _visibleShots;
    std::vector<std::shared_ptr<Shader>> _shadersForPointLights;
    std::shared_ptr<Camera> _cameraRef;
    float _bulletSpeed = 4.0f;

    bool isSpotActive = true;

    void UpdateActiveShots();
    void UpdateGlowsticks();
    void SortActiveShots();
    void UpdateLights();
    void InitShadersForPointLights();
    void SortVisibleLights(std::vector<VisibleLight>& lights);
    void UpdateShadersPointLights(const std::vector<VisibleLight>& pointLights);
    void UpdateShaderPointLight(std::shared_ptr<Shader> shader, string name, glm::vec3 pos, glm::vec3 color, LightSource lightSource);

    void GenerateRandomColors(int amount);
    glm::vec3 GenerateRandomColor();
    glm::vec3 GetRandomColor();
};