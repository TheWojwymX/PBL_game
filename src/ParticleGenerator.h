#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include "Core/Node.h"
#include "texture.h"
#include "ComputeShader.h"
#include <memory>
#include <vector>
#include "glm/glm.hpp"


struct Particle {
    glm::vec4 Position;
    glm::vec4 Velocity;
    glm::vec4 Color;
    float Life;
    float Scale;
    float pad[2];

    Particle()
            : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f), Scale(0.5f), pad{0.0f,0.0f} {}
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

    bool generateParticle = false;

private:
    std::shared_ptr<Shader> shader;
    std::shared_ptr<ComputeShader> computeShader;
    std::string particleType;
    Texture2D texture;

    unsigned int VAO, particleBuffer, instanceVBO;
    glm::vec3 offset{0.0f};
    std::shared_ptr<Node> object;
    std::vector<Particle> particles;

    float elapsedTime = 0.0f;
    float spawnDelay = 1.0f;
    float speedVariation = 0.2f;
    float particleLife = 1.0f;
    unsigned int amount = 500;
    unsigned int newParticles = 10;
    float XZvariation = 0.1f;
    glm::vec4 particleColor;
    float initialUpwardBoost;
    float particleScale;

    glm::vec3 camPosition;
    glm::vec3 camForward;

    void initiateParticleType();
};

#endif