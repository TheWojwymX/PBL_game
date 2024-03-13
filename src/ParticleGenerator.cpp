#include "ParticleGenerator.h"

ParticleGenerator::ParticleGenerator(Shader* shader, Texture2D texture, unsigned int amount)
    : shader(shader), texture(texture), amount(amount)
{
    this->init();
}

void ParticleGenerator::Update(float dt, Node* object, unsigned int newParticles, glm::vec3 offset)
{
    // Increment elapsed time
    this->elapsedTime += dt;

    // Add new particles with a delay
    if (this->elapsedTime >= this->spawnDelay)
    {
        for (unsigned int i = 0; i < newParticles; ++i)
        {
            int unusedParticle = this->firstUnusedParticle();
            this->respawnParticle(this->particles[unusedParticle], object, offset);
        }
        // Reset elapsed time
        this->elapsedTime = 0.0f;
    }

    // Update all particles
    for (unsigned int i = 0; i < this->amount; ++i)
    {
        Particle& p = this->particles[i];
        p.Life -= dt; // reduce life
        if (p.Life > 0.0f)
        {	// particle is alive, thus update
            p.Position += p.Velocity * dt;
            p.Color.a -= dt * 0.25f;
        }
    }
}

void ParticleGenerator::Draw(glm::mat4 viewMatrix)
{
    // Use additive blending to give it a 'glow' effect
    glBlendFunc(GL_SRC_ALPHA, GL_ONE);
    this->shader->use();

    // Determine the camera's position and forward vector in world space
    glm::vec3 cameraPosition = glm::vec3(viewMatrix[3]);
    glm::vec3 cameraForward = -glm::normalize(glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]));

    // Calculate distances from the camera along the view direction for each particle
    std::vector<std::pair<float, Particle>> distanceParticlePairs;
    for (const Particle& particle : this->particles)
    {
        if (particle.Life > 0.0f)
        {
            glm::vec3 particleToCamera = cameraPosition - particle.Position;
            float distanceAlongView = glm::dot(particleToCamera, cameraForward);
            distanceParticlePairs.push_back(std::make_pair(distanceAlongView, particle));
        }
    }

    // Sort particles based on distance in ascending order (back to front)
    std::sort(distanceParticlePairs.begin(), distanceParticlePairs.end(),
        [](const std::pair<float, Particle>& a, const std::pair<float, Particle>& b) {
            return a.first < b.first;
        });

    // Draw particles in the sorted order
    for (const auto& pair : distanceParticlePairs)
    {
        const Particle& particle = pair.second;
        this->shader->setVec3("offset", particle.Position);
        this->shader->setVec4("color", particle.Color);
        this->texture.Bind();
        glBindVertexArray(this->VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);
        glBindVertexArray(0);
    }

    // Don't forget to reset to default blending mode
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::init()
{
    this->spawnDelay = 1.0f;
    this->elapsedTime = 0.0f;

    // set up mesh and attribute properties
    unsigned int VBO;
    float particle_quad[] = {
        -0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, -0.5f, 1.0f, 0.0f,
        -0.5f, -0.5f, 0.0f, 0.0f,

        -0.5f, 0.5f, 0.0f, 1.0f,
        0.5f, 0.5f, 1.0f, 1.0f,
        0.5f, -0.5f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(this->VAO);
    // fill mesh buffer
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);
    // set mesh attributes
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), (void*)0);
    glBindVertexArray(0);

    // create this->amount default particle instances
    for (unsigned int i = 0; i < this->amount; ++i)
        this->particles.push_back(Particle());
}

// stores the index of the last particle used (for quick access to next dead particle)
unsigned int lastUsedParticle = 0;
unsigned int ParticleGenerator::firstUnusedParticle()
{
    // first search from last used particle, this will usually return almost instantly
    for (unsigned int i = lastUsedParticle; i < this->amount; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // otherwise, do a linear search
    for (unsigned int i = 0; i < lastUsedParticle; ++i) {
        if (this->particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // all particles are taken, override the first one (note that if it repeatedly hits this case, more particles should be reserved)
    lastUsedParticle = 0;
    return 0;
}

void ParticleGenerator::respawnParticle(Particle& particle, Node* object, glm::vec3 offset)
{
    float rColor = 0.5f;
    particle.Position = offset;
    particle.Color = glm::vec4(rColor, rColor, rColor, 1.0f);
    particle.Life = 4.0f;

    // Add randomness to velocity (within +- 20%)
    float speedVariation = 0.2f;
    float speedMultiplier = 1.0f + (2.0f * speedVariation * (std::rand() / (float)RAND_MAX) - speedVariation);
    float randomX = (2.0f * (std::rand() / (float)RAND_MAX) - 1.0f) * 0.1f;
    float randomZ = (2.0f * (std::rand() / (float)RAND_MAX) - 1.0f) * 0.1f;
    particle.Velocity = glm::vec3(randomX, 0.75f * speedMultiplier, randomZ);
}