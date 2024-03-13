#ifndef PARTICLE_GENERATOR_H
#define PARTICLE_GENERATOR_H

#include "Node.h"
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
class ParticleGenerator
{
public:
    // constructor
    ParticleGenerator(Shader* shader, Texture2D texture, unsigned int amount);
    // update all particles
    void Update(float dt, Node* object, unsigned int newParticles, glm::vec3 offset = glm::vec3(0.0f));
    // render all particles
    void Draw(glm::mat4 viewMatrix);
private:
    // state
    std::vector<Particle> particles;
    unsigned int amount;
    // render state
    Shader* shader;
    Texture2D texture;
    unsigned int VAO;
    float elapsedTime;
    float spawnDelay;
    // initializes buffer and vertex attributes
    void init();
    // returns the first Particle index that's currently unused e.g. Life <= 0.0f or 0 if no particle is currently inactive
    unsigned int firstUnusedParticle();
    // respawns particle
    void respawnParticle(Particle& particle, Node* object, glm::vec3 offset = glm::vec3(0.0f));
};

#endif