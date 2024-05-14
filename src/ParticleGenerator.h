#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include "Core/Node.h"
#include "texture.h"

// Represents a single particle and its state
struct Particle {
    glm::vec3 Position, Velocity;
    glm::vec4 Color;
    float     Life;

    Particle() : Position(0.0f), Velocity(0.0f), Color(1.0f), Life(0.0f) { }
};

// ParticleGenerator acts as a container for rendering a large number of
// particles by repeatedly spawning and updating particles and killing
// them after a given amount of time.
class ParticleGenerator : public Component
{
public:
    ParticleGenerator();
    // constructor
    ParticleGenerator(shared_ptr<Shader> shader, string particleType);
    // update all particles
    void Update() override;
    // render all particles
    void Render(glm::mat4 parentWorld) override;

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json& jsonData) override;

    void initiate() override;
    void Init() override;
private:
    // state
    std::vector<Particle> particles;
    unsigned int amount;
    // render state
    shared_ptr<Shader> shader;
    shared_ptr<Node> object;
    int newParticles;
    float speedVariation;
    float particleLife;
    Texture2D texture;
    string particleType;
    glm::vec3 offset;
    unsigned int VAO;
    float elapsedTime;
    float spawnDelay;
    // initializes buffer and vertex attributes

    // returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    unsigned int firstUnusedParticle();
    // respawns particle
    void respawnParticle(Particle& particle);
    void initiateParticleType();

};

#endif