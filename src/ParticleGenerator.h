#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include "Core/Node.h"
#include "texture.h"
#include "ComputeShader.h"
#include <memory>
#include <vector>
#include "glm/glm.hpp"
#include <glm/gtx/quaternion.hpp>
#include "Managers/NodesManager.h"
#include "WindSimulation.h"
#include "Managers/WeatherManager.h"


struct Particle {
    glm::vec4 Position;
    glm::vec4 Velocity;
    float Life;
    float Scale;
    float Ground;
    float Weight;

    Particle()
            : Position(0.0f), Velocity(0.0f), Life(0.0f), Scale(0.5f), Ground(0.0f), Weight(1.0f) {}
};

class ParticleGenerator : public Component {
public:
    ParticleGenerator(std::shared_ptr<Shader> shader, std::string particleType);
    ParticleGenerator();

    nlohmann::json Serialize() override;
    void Deserialize(const nlohmann::json& jsonData) override;

    void UpdateParticles();
    void RenderParticles();
    void Update() override;
    void Render(glm::mat4 parentWorld) override;
    void Init() override;

    void SetOffset(glm::vec3 newOffset) { offset = newOffset; }
    void SpawnParticles();

    bool generateParticle = false;
    std::shared_ptr<Node> object;
    std::shared_ptr<Shader> shader;
    std::string particleType = "noName";

    glm::vec3 enemyScale;
    glm::vec3 enemyPosition;
    glm::vec3 generatorPosition;
    unsigned int newParticles = 10;
    unsigned int amount = 500;
    bool toDelete = false;
    glm::vec3 jumpOffPoint{0.0f};
    glm::vec3 gravity;
    bool tooltipShrink = false;
    bool tooltipSpawn = true;
private:

    std::shared_ptr<ComputeShader> computeShader;

    Texture2D texture;

    bool firstPass = true;
    unsigned int VAO, VBO, particleBuffer;
    Particle* particleData;
    GLuint instanceVBOs[3];
    char* instanceData[3];
    int currentBuffer = 0;

    glm::vec3 offset{0.0f};
    std::vector<Particle> particles;

    float elapsedTime = 0.0f;
    float spawnDelay = 1.0f;
    float speedVariation = 0.2f;
    float particleLife = 1.0f;


    bool counterXrotation = false;
    bool isFlare = false;

    float XZvariation = 0.1f;
    glm::vec4 particleColor;
    float initialUpwardBoost;
    float particleScale;


    bool hasSpawned = false;

    glm::vec3 camPosition;
    glm::vec3 camForward;
    glm::vec3 rotatedOffset;

    bool onlyForward = false;
    bool casing = false;
    bool ambient = false;
    bool rain = false;
    bool isUnderground = false;
    bool isJetpack = false;
    bool tooltip = false;
    bool showTexture = false;

    int threadCount = 0;

    void initiateParticleType();

    void SetInitialUpwardBoost(const glm::vec3& scale, float bonus);
    void SetPartScale(const glm::vec3& scale, float bonus);
};

#endif