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
uniform mat4 objectRotation;
uniform float particleLife;
uniform float speedVariation;
uniform float dt;
uniform float spawnDelay;
uniform int amount;
uniform int newParticles;
uniform float XZvariation;
uniform vec4 particleColor;
uniform bool shouldSpawn;
uniform vec3 gravity;
uniform float initialUpwardBoost;
uniform float particleScale;
uniform vec3 cameraPosition;
uniform vec3 cameraForward;
uniform bool hasSpawned;
uniform bool onlyForward;
uniform bool casing;
uniform vec3 enemyPosition;
uniform bool isJetpack;
uniform vec3 jumpOff;

layout (local_size_x = 1) in;

uint lastUsedParticle = 0;
vec3 initialVel;

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

    vec3 initialVelocity;

    vec3 directionToEnemy = normalize(enemyPosition - nodePosition);

    float speedMultiplier = 1.0f + (speedVariation * random(changeSeed + 1.0f));

    if(onlyForward)
    {
        initialVelocity = vec3(0.0, directionToEnemy.y * 40, XZvariation);
    }
    else if(casing)
    {
        initialVelocity = vec3(0.7f, speedMultiplier + initialUpwardBoost, 0);
    }
    else
    {
    float randomX = (2.0f * random(changeSeed + 2.0f) - 1.0f) * XZvariation;
    float randomZ = (2.0f * random(changeSeed + 3.0f) - 1.0f) * XZvariation;

    initialVelocity = vec3(randomX, speedMultiplier + initialUpwardBoost, randomZ);
    }

    initialVelocity = (objectRotation * vec4(initialVelocity, 0.0f)).xyz;

    initialVel = initialVelocity;
    particle.Velocity.xyz = initialVelocity;
    particle.Velocity.w = 0.0f;
}

void main() {
    uint id = gl_GlobalInvocationID.x;
    vec3 initGravity = gravity;
    if (id < amount) {
        Particle p = particles[id];

        if (p.Life > 0.0f) {
            //if(dot(cameraForward, normalize(nodePosition - cameraPosition)) > 0.6)
            //{
            // Update velocity with gravity
            p.Velocity.xyz += initGravity * dt;
            // Update position with velocity
            p.Position.xyz += p.Velocity.xyz * dt;
            // Decrease alpha to fade out particle

            // Reduce life time
            p.Life -= dt;

            float lifeRatio = p.Life / particleLife;
            p.Scale = mix(0.1f, particleScale, lifeRatio);

            // Check for bounce when particle hits y = 105
            if (p.Position.y <= 100.0 && !isJetpack) {
                if(onlyForward)
                {
                p.Life -= 4 * dt;
                }
                else{
                // Reflect the y velocity for a bounce and dampen it to lose energy
                p.Velocity.y = -p.Velocity.y * 0.8; // Adjust 0.8 damping factor as needed
                p.Velocity.x = p.Velocity.x * 0.8;
                p.Velocity.z = p.Velocity.z * 0.8;

                // Set position exactly at 100 to prevent particles from going below it
                p.Position.y = 100.0 + (100.0 - p.Position.y);
                }
            }
            else if(p.Position.y <= jumpOff.y + 0.2 && isJetpack){
                initGravity.y = 0.0;
                p.Velocity.y = 0.0;
                p.Position.y = jumpOff.y + 0.2;
                p.Velocity.x = p.Velocity.x * 0.8;
                p.Velocity.z = p.Velocity.z * 0.8;
            }
            //}
            //else{
            //p.Life = 0.0;
            //}
        }

        particles[id] = p;
    }

    // Handle adding new particles
    if (id == 0 && shouldSpawn && !hasSpawned) {
        for (int i = 0; i < newParticles; ++i) {
            uint unusedParticle = firstUnusedParticle();
            if (unusedParticle != uint(-1)) {
                respawnParticle(particles[unusedParticle], unusedParticle, float(i));
            }
        }
    }
}