#include "ParticleGenerator.h"
#include <glad/glad.h>
#include <vector>
#include <algorithm>

ParticleGenerator::ParticleGenerator(std::shared_ptr<Shader> shader, string particleType)
        : shader(shader), particleType(particleType) {}

ParticleGenerator::ParticleGenerator() {
    _type = ComponentType::PARTICLEGENERATOR;
}

nlohmann::json ParticleGenerator::Serialize() {
    nlohmann::json data = Component::Serialize();

    data["offset"] = {
            {"x", offset.x},
            {"y", offset.y},
            {"z", offset.z}
    };
    data["particleType"] = particleType;
    data["shader"] = shader->_name;

    return data;
}

void ParticleGenerator::Deserialize(const nlohmann::json &jsonData) {
    if (jsonData.contains("offset")) {
        offset.x = jsonData["offset"]["x"].get<float>();
        offset.y = jsonData["offset"]["y"].get<float>();
        offset.z = jsonData["offset"]["z"].get<float>();
    }

    if (jsonData.contains("particleType")) {
        particleType = jsonData["particleType"].get<string>();
    }

    if (jsonData.contains("shader")) {
        shader = RESOURCEMANAGER.GetShaderByName(jsonData["shader"].get<string>());
    }

    Component::Deserialize(jsonData);
}

void ParticleGenerator::Update() {
    if(generateParticle) UpdateParticles();
}

void ParticleGenerator::Render(glm::mat4 parentWorld){
    if(generateParticle) RenderParticles();
}

void ParticleGenerator::UpdateParticles() {
    float dt = TIME.GetDeltaTime();
    elapsedTime += dt;
    bool shouldSpawn = false;
    if(elapsedTime >= spawnDelay)
    {
        shouldSpawn = true;
        elapsedTime = 0.0f;
    }

    camForward = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetFrontVector();
    camPosition = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition();

    computeShader->use();
    computeShader->setFloat("dt", dt);
    computeShader->setVec3("nodePosition", object->GetTransform()->GetPosition() + offset);
    computeShader->setFloat("elapsedTime", elapsedTime);
    computeShader->setBool("shouldSpawn", shouldSpawn);
    computeShader->setVec3("cameraPosition", camPosition);
    computeShader->setVec3("cameraForward", camForward);

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleBuffer);
    glDispatchCompute(amount, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ParticleGenerator::RenderParticles() {
    glm::mat4 viewMatrix = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetViewMatrix();

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    shader->use();

    glm::vec3 cameraPosition = glm::vec3(viewMatrix[3]);
    glm::vec3 cameraForward = -glm::normalize(glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]));


    std::vector<std::pair<float, Particle>> distanceParticlePairs;
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleBuffer);
    Particle* particleData = (Particle*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, amount * sizeof(Particle), GL_MAP_READ_BIT);

    for (unsigned int i = 0; i < amount; ++i) {
        Particle& particle = particleData[i];
        if (particle.Life > 0.0f) {
            glm::vec3 particleToCamera = glm::vec3(cameraPosition.x - particle.Position.x,cameraPosition.y - particle.Position.y, cameraPosition.z - particle.Position.z);
            float distanceAlongView = glm::dot(particleToCamera, cameraForward);
            distanceParticlePairs.push_back({ distanceAlongView, particle });
        }
    }

    glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

    std::sort(distanceParticlePairs.begin(), distanceParticlePairs.end(),
              [](const std::pair<float, Particle>& a, const std::pair<float, Particle>& b) {
                  return a.first < b.first;
              });

    for (const auto& pair : distanceParticlePairs) {
        const Particle& particle = pair.second;
        if(particle.Life > 0.0f) {
            shader->setVec3("offset", glm::vec3(particle.Position.x, particle.Position.y, particle.Position.z));
            shader->setVec4("color", particle.Color);
            shader->setFloat("scale", particle.Scale);
            texture.Bind();
            glBindVertexArray(VAO);
            glDrawArrays(GL_TRIANGLES, 0, 6);
            glBindVertexArray(0);
        }
    }

    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}

void ParticleGenerator::Init() {
    computeShader = RESOURCEMANAGER.GetComputeShaderByName("particleComputeShader");
    object = this->GetOwnerNode();
    initiateParticleType();
    Component::initiate();

    elapsedTime = 0.0f;

    computeShader->use();
    computeShader->setFloat("particleLife", particleLife);
    computeShader->setFloat("speedVariation", speedVariation);
    computeShader->setFloat("spawnDelay", spawnDelay);
    computeShader->setInt("amount", amount);
    computeShader->setInt("newParticles", newParticles);
    computeShader->setFloat("XZvariation", XZvariation);
    computeShader->setVec4("particleColor", particleColor);
    computeShader->setFloat("initialUpwardBoost", initialUpwardBoost);
    computeShader->setFloat("particleScale", particleScale);


    unsigned int VBO;
    float particle_quad[] = {
            -0.5f, 0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 1.0f, 0.0f
    };
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);
    glBindVertexArray(0);

    particles.resize(amount);
    glGenBuffers(1, &particleBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, amount * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleBuffer);
}

void ParticleGenerator::initiateParticleType() {
    if (particleType == "antWalk") {
        texture = Texture2D::loadTextureFromFile("../../res/Particle/particle.png", true);
        amount = 100;
        newParticles = 10;
        spawnDelay = 0.2f;
        speedVariation = 0.2f;
        XZvariation = 0.5f;
        particleLife = 4.0f;
        particleColor = glm::vec4(1.0f,0.1f,0.1f,1.0f);
        initialUpwardBoost = 3.0f;
        particleScale = 0.3f;
    }
    else if (particleType == "secondType"){
        texture = Texture2D::loadTextureFromFile("../../res/Particle/particle.png", true);
        amount = 500;
        newParticles = 10;
        spawnDelay = 1.0f;
        speedVariation = 0.2f;
        XZvariation = 0.1f;
        particleLife = 1.0f;
        particleColor = glm::vec4(1.0f,0.1f,0.1f,1.0f);
        initialUpwardBoost = 3.0f;
        particleScale = 0.5f;
    }
}