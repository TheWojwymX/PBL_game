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
}

void WeatherManager::Update(){
    windDirection = windModel.getWind(15 * TIME.GetDeltaTime());
    windDirection.x /= 10;
    windDirection.y /= 10;
    windDirection.z /= 10;
    if(windDirection.y > 0.1) windDirection.y = 0.1;
    if(windDirection.y < -0.1) windDirection.y = -0.1;
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