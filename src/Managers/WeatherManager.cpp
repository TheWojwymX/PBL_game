#include "WeatherManager.h"


WeatherManager &WeatherManager::getInstance() {
    static WeatherManager instance;
    return instance;
}

void WeatherManager::Init() {
    windDirection = glm::vec3(0.0f,0.0f,0.0f);
    windStrength = 0.7f;
    windModel.initialize(0.1, 1.0, 0.1, 1.0, 1.0, 1.0, 2.0);

    SetupAmbientParticles();
    SetupRainParticles();
}

void WeatherManager::Update(){
    windDirection = windModel.getWind(15 * TIME.GetDeltaTime());
    windDirection.x /= 10;
    windDirection.y /= 10;
    windDirection.z /= 10;
    if(windDirection.y > 0.1) windDirection.y = 0.1;
    if(windDirection.y < -0.1) windDirection.y = -0.1;

    timeSinceLastCheck += TIME.GetDeltaTime();
    if(timeSinceLastCheck >= rainCheckInterval)
    {
        timeSinceLastCheck = 0.0f;

        if (!isRaining) {
            spawnedParticles = 0;
            float chance = static_cast<float>(std::rand()) / RAND_MAX;
            if (chance < rainProbability) {
                isRaining = true;
                rainTimeLeft = rainDuration;
            }
        }
    }
    // Update rain duration
    if (isRaining) {
        cout <<"pada" << endl;
        rainTimeLeft -= TIME.GetDeltaTime();
        timeSinceLastParticleSpawn += TIME.GetDeltaTime();
        auto particleGenerator = NODESMANAGER.getNodeByName("RainParticles")->GetComponent<ParticleGenerator>();
        auto maxParticles = particleGenerator->newParticles;
        if(spawnedParticles < particleGenerator->amount) {
            if (rainTimeLeft >= (2 * rainDuration / 3) && timeSinceLastParticleSpawn >= 10 * particleSpawnInterval) {
                windDirection.y = -0.5f;
                particleGenerator->gravity = glm::vec3(0.0f, -2.0f, 0.0f);
                particleGenerator->SpawnParticles(); //spawn once
                timeSinceLastParticleSpawn = 0.0f;
            } else if (rainTimeLeft < 2 * rainDuration / 3 && timeSinceLastParticleSpawn >= particleSpawnInterval) {
                windDirection.y = -1.5f;
                particleGenerator->gravity = glm::vec3(0.0f, -5.0f, 0.0f);
                NODESMANAGER.getNodeByName("RainParticles")->GetComponent<ParticleGenerator>()->SpawnParticles();
                timeSinceLastParticleSpawn = 0.0f;
            } else if (rainTimeLeft < rainDuration / 3 && timeSinceLastParticleSpawn >= 10 * particleSpawnInterval) {
                windDirection.y = -0.5f;
                particleGenerator->gravity = glm::vec3(0.0f, -2.0f, 0.0f);
                particleGenerator->SpawnParticles(); //spawn once
                timeSinceLastParticleSpawn = 0.0f;
            }
        }
        if (rainTimeLeft <= 0.0f) {
            isRaining = false;
        }
    }
}

void WeatherManager::SetupAmbientParticles() {
    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), "AmbientParticles");
    auto node = NODESMANAGER.getNodeByName("AmbientParticles");
    node->GetTransform()->SetPosition(glm::vec3(50.0f,299.0f,50.0f));

    auto ambientParticles = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"), "ambientSandParticles");
    ambientParticles->SetOffset(glm::vec3(0.0f, 0.5f, 0.0f));
    ambientParticles->object = node;
    ambientParticles->Init();
    node->AddComponent(ambientParticles);

    ambientParticles->SpawnParticles();
}

void WeatherManager::SetupRainParticles() {
    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), "RainParticles");
    auto node = NODESMANAGER.getNodeByName("RainParticles");
    node->GetTransform()->SetPosition(glm::vec3(53.0f,299.0f,50.0f));

    auto rainParticles = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"), "rainParticles");
    rainParticles->SetOffset(glm::vec3(0.0f, 0.5f, 0.0f));
    rainParticles->object = node;
    rainParticles->Init();
    node->AddComponent(rainParticles);

    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), "RainParticles2");
    auto node2 = NODESMANAGER.getNodeByName("RainParticles2");
    node2->GetTransform()->SetPosition(glm::vec3(53.0f,299.0f,50.0f));

    auto rainParticles2 = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"), "rainParticlesFull");
    rainParticles2->SetOffset(glm::vec3(0.0f, 0.5f, 0.0f));
    rainParticles2->object = node2;
    rainParticles2->Init();
    node2->AddComponent(rainParticles2);
}