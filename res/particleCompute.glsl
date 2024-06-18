#version 430

struct Particle {
    vec4 Position;
    vec4 Velocity;
    float Life;
    float Scale;
    float Ground;
    float Weight;
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
uniform float groundLevel;
uniform vec3 windDirection;
uniform float windStrength;
uniform bool isFlare;
uniform bool ambient;
uniform bool isUnderground;
uniform bool rain;
uniform bool tooltip;
uniform bool tooltipSpawn;
uniform bool tooltipShrink;

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
    particle.Life = particleLife;
    particle.Scale = particleScale;
    particle.Ground = jumpOff.y;
    if(ambient){
    particle.Weight = (random(changeSeed) * 3) + 0.1 ;
    }
    else{
    particle.Weight = 1.0;
    }

    vec3 initialVelocity;

    vec3 directionToEnemy = normalize(enemyPosition - nodePosition);

    float speedMultiplier = 1.0f + (speedVariation * random(changeSeed + 1.0f));

    if(onlyForward)
    {
        initialVelocity = vec3(directionToEnemy.x * 40, (directionToEnemy.y + 0.09) * 40, directionToEnemy.z * 40);
    }
    else if(casing)
    {
        initialVelocity = vec3(0.7f, speedMultiplier + initialUpwardBoost, 0);
    }
    else if(!tooltip)
    {
    float randomX = (2.0f * random(changeSeed + 2.0f) - 1.0f) * XZvariation;
    float randomZ = (2.0f * random(changeSeed + 3.0f) - 1.0f) * XZvariation;

    initialVelocity = vec3(randomX, speedMultiplier + initialUpwardBoost, randomZ);
    initialVelocity = (objectRotation * vec4(initialVelocity, 0.0f)).xyz;
    }
    else if(tooltip){
        initialVelocity = vec3(0.0, 0.0, 0.0);
    }


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
            if(!tooltip){
            p.Velocity.xyz += (initGravity + ((windDirection * windStrength) * p.Weight)) * dt;
            // Update position with velocity
            p.Position.xyz += p.Velocity.xyz * dt;
            // Decrease alpha to fade out particle

            // Reduce life time
            p.Life -= dt;

            float lifeRatio = p.Life / particleLife;
            if(!isFlare){
            p.Scale = mix(0.05f, particleScale, lifeRatio);
            }
            else{
            if (p.Life > particleLife / 3) {
                    p.Scale = mix(1.7f, particleScale, lifeRatio);
                } else {
                    p.Scale = mix(particleScale, 3.2, lifeRatio);
                }
            }
}
if(tooltip){
if(p.Scale < 1.5 && !tooltipShrink){
p.Scale += dt * 10;
}
if(tooltipShrink && p.Scale > 0.05){
p.Scale -= dt * 10;
}
}
if(!tooltipSpawn)
{
p.Life = 0.0;
}

if(p.Scale > 5.0){
p.Scale = 0.0;
p.Position.xyz = vec3(-300.0,-300.0,-300.0);
p.Life = 0.0;
}
            // Check for bounce when particle hits y = 105
            if (p.Position.y <= groundLevel && !isJetpack && !isFlare && !isUnderground) {
                if(onlyForward)
                {
                p.Life -= 4 * dt;
                }
                else{
                // Reflect the y velocity for a bounce and dampen it to lose energy

                if(!ambient){
                if(!casing){
                p.Velocity.y = -p.Velocity.y * 0.8; // Adjust 0.8 damping factor as needed
                }
                else{
                p.Velocity.y = -p.Velocity.y * 0.1;
                }
                p.Velocity.x = p.Velocity.x * 0.8;
                p.Velocity.z = p.Velocity.z * 0.8;
                }
                if(ambient){
                p.Velocity.y = -p.Velocity.y * 0.2;
                }
                if(rain){
                p.Velocity.y = -p.Velocity.y * 0.1;
                }

                // Set position exactly at 100 to prevent particles from going below it
                p.Position.y = groundLevel + (groundLevel - p.Position.y);
                }
            }
            else if(p.Position.y <= p.Ground - 0.5 + 0.2 && isJetpack){
                initGravity.y = 0.0;
                p.Velocity.y = 0.0;
                if(p.Ground <= p.Position.y)
                {
                p.Position.y = p.Ground - 0.5 + 0.2;
                }
                p.Velocity.x = p.Velocity.x * 0.8;
                p.Velocity.z = p.Velocity.z * 0.8;
            }

            // Check for bounce when particle hits the circle boundary
            vec3 toParticle = p.Position.xyz - vec3(49.5,300,49.5);
            float distanceToCenter = length(toParticle);
            if (distanceToCenter < 15.0 && !isJetpack && !isFlare && !isUnderground && !tooltip && !rain && !casing && !onlyForward) {
                vec3 normal = normalize(toParticle);
                p.Position.xyz = vec3(49.5,300,49.5) + normal * 15.0;
                p.Velocity.xyz = reflect(p.Velocity.xyz, normal) * 0.8; // Adjust 0.8 damping factor as needed
            }

            //}
            //else{
            //p.Life = 0.0;
            //}
            if(ambient){
            if(p.Life < particleLife - 2.05){
                if(!rain){
                p.Life = particleLife - 2.3;
                }
                p.Velocity.xyz = p.Velocity.xyz * 0.999;
            }
            else if(p.Life < particleLife - 2.0){
            p.Velocity = vec4(0.0, 0.0, 0.0, 0.0);
            p.Scale = particleScale/2;
            }
            else if(p.Life > particleLife - 2.0){
            p.Scale = 0.0;
            }

            if(!rain){
                if(p.Position.x > 120){
                p.Position.x = -20;
                }

                if(p.Position.z > 120){
                p.Position.z = -20;
                }

                if(p.Position.x < -20){
                p.Position.x = 120;
                }

                if(p.Position.z < -20){
                p.Position.z = 120;
                }

                if(p.Position.y < 299){
                p.Position.y = 400;
                }

                if(p.Position.y > 310){
                p.Position.y = 300;
                }
            }
            else{
                if(p.Position.x > 90){
                p.Position.x = 10;
                }

                if(p.Position.z > 90){
                p.Position.z = 10;
                }

                if(p.Position.x < 10){
                p.Position.x = 90;
                }

                if(p.Position.z < 10){
                p.Position.z = 90;
                }


                if (p.Position.y < 300 && p.Velocity.y < -1.0){
                p.Velocity.y = -p.Velocity.y * 0.1;
                }
                if(p.Position.y < 300 && p.Velocity.y > -5.0){
                p.Position.y = 330;
                }

                if(p.Position.y > 330){
                p.Position.y = 300;
                }
            }


            }
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