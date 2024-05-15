#version 430

struct Particle {
    vec4 Position;
    vec4 Velocity;
    vec4 Color;
    float Life;
    float Scale;
};

layout (std430, binding = 0) buffer Particles {
    Particle particles[];
};

uniform vec3 nodePosition;
uniform float particleLife;
uniform float speedVariation;
uniform float dt;
uniform float spawnDelay;
uniform int amount;
uniform int newParticles;
uniform float XZvariation;
uniform vec4 particleColor;
uniform bool shouldSpawn;
uniform vec3 gravity = vec3(0.0, -9.81, 0.0); // Gravity vector, feel free to adjust
uniform float initialUpwardBoost;
uniform float particleScale;
uniform vec3 cameraPosition;
uniform vec3 cameraForward;

layout (local_size_x = 1) in;

uint lastUsedParticle = 0;

float random(float seed) {
    return fract(sin(seed) * 43758.5453);
}

uint firstUnusedParticle() {
    // Start searching from `lastUsedParticle`
    for (uint i = lastUsedParticle; i < amount; ++i) {
        if (particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // If not found, wrap around
    for (uint i = 0; i < lastUsedParticle; ++i) {
        if (particles[i].Life <= 0.0f) {
            lastUsedParticle = i;
            return i;
        }
    }
    // Return uint(-1) if no inactive particle is found
    return uint(-1);
}

void respawnParticle(inout Particle particle, uint index, float seed) {
    float changeSeed = float(index) + seed;
    particle.Position.xyz = nodePosition;
    particle.Position.w = 0.0f;
    particle.Color = particleColor;
    particle.Life = particleLife;
    particle.Scale = particleScale;

    float speedMultiplier = 1.0f + (speedVariation * random(changeSeed + 1.0f));
    float randomX = (2.0f * random(changeSeed + 2.0f) - 1.0f) * XZvariation;
    float randomZ = (2.0f * random(changeSeed + 3.0f) - 1.0f) * XZvariation;
    particle.Velocity.xyz = vec3(randomX, speedMultiplier + initialUpwardBoost, randomZ);
    particle.Velocity.w = 0.0f;
}

void main() {
    uint id = gl_GlobalInvocationID.x;

    if (id < amount) {
        Particle p = particles[id];

        if (p.Life > 0.0f) {
            if(dot(cameraForward, normalize(nodePosition - cameraPosition)) > 0.6)
            {
            // Update velocity with gravity
            p.Velocity.xyz += gravity * dt;
            // Update position with velocity
            p.Position.xyz += p.Velocity.xyz * dt;
            // Decrease alpha to fade out particle
            if(p.Scale > 0.0) {
            p.Scale -= dt * 0.07f;
            }
            // Reduce life time
            p.Life -= dt;

            // Check for bounce when particle hits y = 105
            if (p.Position.y <= 100.0) {
                // Reflect the y velocity for a bounce and dampen it to lose energy
                p.Velocity.y = -p.Velocity.y * 0.8; // Adjust 0.8 damping factor as needed
                // Set position exactly at 100 to prevent particles from going below it
                p.Position.y = 100.0 + (100.0 - p.Position.y);
            }
            }
            else{
            p.Life = 0.0;
            }
        }

        particles[id] = p;
    }

    // Handle adding new particles
    if (id == 0 && shouldSpawn) {
        for (int i = 0; i < newParticles; ++i) {
            uint unusedParticle = firstUnusedParticle();
            if (unusedParticle != uint(-1)) {
                respawnParticle(particles[unusedParticle], unusedParticle, float(i));
            }
        }
    }
}