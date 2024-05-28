#include "ParticleGenerator.h"
#include <glad/glad.h>
#include <vector>
#include <algorithm>

ParticleGenerator::ParticleGenerator(std::shared_ptr<Shader> shader, string particleType)
        : shader(shader), particleType(particleType) {}

ParticleGenerator::ParticleGenerator() : shader(nullptr), particleType("default") {
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
    if(generateParticle)
    {
        UpdateParticles();
    }
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

    glm::quat rotationQuat = object->GetTransform()->GetRotation();
    glm::mat4 rotationMatrix = glm::toMat4(rotationQuat);

    // Adjust `offset` based on the rotation
    rotatedOffset = glm::vec3(rotationMatrix * glm::vec4(offset, 1.0f));

    computeShader->use();
    computeShader->setFloat("dt", dt);
    computeShader->setVec3("nodePosition", object->GetTransform()->GetPosition() + rotatedOffset);
    computeShader->setFloat("elapsedTime", elapsedTime);
    computeShader->setBool("shouldSpawn", shouldSpawn);
    computeShader->setVec3("cameraPosition", camPosition);
    computeShader->setVec3("cameraForward", camForward);
    computeShader->setBool("hasSpawned", hasSpawned);
    computeShader->setMat4("objectRotation", glm::toMat4(object->GetTransform()->GetRotation()));
    computeShader->setVec3("enemyPosition", enemyPosition);
    computeShader->setVec3("jumpOff", jumpOffPoint);

    generatorPosition = object->GetTransform()->GetPosition() + rotatedOffset;

    hasSpawned = true;


    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleBuffer);
    glDispatchCompute(amount, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ParticleGenerator::RenderParticles() {
    //bool visible = glm::dot(camForward, glm::normalize(object->GetTransform()->GetPosition() + rotatedOffset) - camPosition) > 0.6f;
    //if((visible && particleType != "turretShot") || particleType == "turretShot") {
        glm::mat4 viewMatrix = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetViewMatrix();
        glm::vec3 cameraPosition = glm::vec3(viewMatrix[3]);
        glm::vec3 cameraForward = -glm::normalize(glm::vec3(viewMatrix[0][2], viewMatrix[1][2], viewMatrix[2][2]));

        std::vector<std::pair<float, unsigned int>> distanceIndexPairs;

        // Map particle buffer
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleBuffer);
        Particle *particleData = (Particle *) glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, amount * sizeof(Particle),
                                                               GL_MAP_READ_BIT);

        int deadParticles = 0;
        for (unsigned int i = 0; i < amount; ++i) {
            Particle &particle = particleData[i];
            if (particle.Life > 0.0f) {
                glm::vec3 particleToCamera = glm::vec3(cameraPosition.x - particle.Position.x,
                                                       cameraPosition.y - particle.Position.y,
                                                       cameraPosition.z - particle.Position.z);
                float distanceAlongView = glm::dot(particleToCamera, cameraForward);
                distanceIndexPairs.push_back({distanceAlongView, i});

            }
            else{
                deadParticles++;
            }
        }

        if (deadParticles == amount)
        {
            generateParticle = false;
        }

        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        // Sort particles by distance
        std::sort(distanceIndexPairs.begin(), distanceIndexPairs.end(),
                  [](const std::pair<float, unsigned int> &a, const std::pair<float, unsigned int> &b) {
                      return a.first < b.first;
                  });

        // Prepare instance data
        std::vector<glm::vec3> offsets;
        std::vector<glm::vec4> colors;
        std::vector<float> scales;

        for (const auto &pair: distanceIndexPairs) {
            const Particle &particle = particleData[pair.second];
            if (particle.Life > 0.0f) {
                offsets.push_back(particle.Position);
                colors.push_back(particle.Color);
                scales.push_back(particle.Scale);
            }
        }

        // Create a single buffer for all instance data
        size_t offsetSize = offsets.size() * sizeof(glm::vec3);
        size_t colorSize = colors.size() * sizeof(glm::vec4);
        size_t scaleSize = scales.size() * sizeof(float);
        size_t totalSize = offsetSize + colorSize + scaleSize;

        glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
        glBufferData(GL_ARRAY_BUFFER, totalSize, nullptr, GL_DYNAMIC_DRAW);

        // Fill the buffer
        glBufferSubData(GL_ARRAY_BUFFER, 0, offsetSize, offsets.data());
        glBufferSubData(GL_ARRAY_BUFFER, offsetSize, colorSize, colors.data());
        glBufferSubData(GL_ARRAY_BUFFER, offsetSize + colorSize, scaleSize, scales.data());

        // Set up instance attributes
        glBindVertexArray(VAO);

        // Instance Position (Location 2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid *) 0);
        glVertexAttribDivisor(2, 1);

        // Instance Color (Location 3)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid *) (offsetSize));
        glVertexAttribDivisor(3, 1);

        // Instance Scale (Location 4)
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (GLvoid *) (offsetSize + colorSize));
        glVertexAttribDivisor(4, 1);

        // Render
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        shader->use();
        texture.Bind();
        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, offsets.size());
        glBindVertexArray(0);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    //}
    if (deadParticles == amount) {
        if (toDelete) {
            GAMEMANAGER.root->removeChild(_ownerNode);
        }
    }
}

void ParticleGenerator::Init() {
    // Initialize compute shader and set its parameters
    computeShader = std::make_shared<ComputeShader>("../../res/particleCompute.glsl", "computeShader");
    if(object == nullptr) object = this->GetOwnerNode();
    initiateParticleType();
    Component::Initiate();

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
    computeShader->setBool("onlyForward", onlyForward);
    computeShader->setVec3("gravity", gravity);
    computeShader->setBool("casing", casing);
    computeShader->setBool("isJetpack", isJetpack);
    computeShader->setFloat("groundLevel", GAMEMANAGER._groundLevel);

    // Particle quad (single instance)
    unsigned int VBO;
    float particle_quad[] = {
            // Position   // TexCoords
            -0.5f, 0.5f, 0.0f, 1.0f,
            0.5f, -0.5f, 1.0f, 0.0f,
            -0.5f, -0.5f, 0.0f, 0.0f,

            -0.5f, 0.5f, 0.0f, 1.0f,
            0.5f, 0.5f, 1.0f, 1.0f,
            0.5f, -0.5f, 1.0f, 0.0f
    };

    // Generate and bind VAO for the particle quad
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(particle_quad), particle_quad, GL_STATIC_DRAW);

    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(float), nullptr);

    // Initialize instance buffer (offsets, colors, scales)
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, amount * sizeof(glm::vec3) + amount * sizeof(glm::vec4) + amount * sizeof(float), nullptr, GL_DYNAMIC_DRAW);

    // Attribute 2: Offsets
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
    glVertexAttribDivisor(2, 1);

    // Attribute 3: Colors
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::vec4), (GLvoid*)(amount * sizeof(glm::vec3)));
    glVertexAttribDivisor(3, 1);

    // Attribute 4: Scales
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (GLvoid*)(amount * sizeof(glm::vec3) + amount * sizeof(glm::vec4)));
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);

    // Initialize particles buffer for compute shader
    particles.resize(amount);
    glGenBuffers(1, &particleBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleBuffer);
    glBufferData(GL_SHADER_STORAGE_BUFFER, amount * sizeof(Particle), particles.data(), GL_DYNAMIC_DRAW);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ParticleGenerator::initiateParticleType() {
    if (particleType == "turretCasing") {
        texture = Texture2D::loadTextureFromFile("../../res/Particle/particle.png", true);
        amount = 100;
        newParticles = 1;
        spawnDelay = 0.0f;
        speedVariation = 0.2f;
        XZvariation = 5.5f;
        particleLife = 2.0f;
        particleColor = glm::vec4(0.6f,0.55f,0.1f,1.0f);
        initialUpwardBoost = 2.0f;
        particleScale = 0.15f;
        gravity = glm::vec3(0.0f, -9.81f, 0.0f);
        onlyForward = false;
        casing = true;
        isJetpack = false;

    }
    else if (particleType == "turretShot"){
        texture = Texture2D::loadTextureFromFile("../../res/Particle/particle.png", true);
        amount = 100;
        newParticles = 1;
        spawnDelay = 0.0f;
        speedVariation = 0.2f;
        XZvariation = 90.0f;
        particleLife = 4.0f;
        particleColor = glm::vec4(0.8f,0.8f,0.0f,1.0f);
        initialUpwardBoost = 0.0f;
        particleScale = 0.25f;
        gravity = glm::vec3(0.0f, 0.0f, 0.0f);
        onlyForward = true;
        casing = false;
        isJetpack = false;
    }
    else if (particleType == "antShot"){
        texture = Texture2D::loadTextureFromFile("../../res/Particle/particle.png", true);
        amount = 50;
        newParticles = 5;
        spawnDelay = 0.0f;
        speedVariation = 0.2f;
        XZvariation = 1.0f;
        particleLife = 3.0f;
        particleColor = glm::vec4(0.3f,0.0f,0.0f,1.0f);
        SetInitialUpwardBoost(enemyScale, 0.0f);
        SetPartScale(enemyScale, 0.0f);
        gravity = glm::vec3(0.0f, -9.81f, 0.0f);
        onlyForward = false;
        casing = false;
        isJetpack = false;
    }
    else if (particleType == "antDie"){
        texture = Texture2D::loadTextureFromFile("../../res/Particle/particle.png", true);
        amount = 30;
        newParticles = 30;
        spawnDelay = 0.0f;
        speedVariation = 1.0f;
        XZvariation = 2.0f;
        particleLife = 6.0f;
        particleColor = glm::vec4(0.545f,0.271f,0.075f,1.0f);
        initialUpwardBoost = 1.0f;
        SetPartScale(enemyScale, 0.0f);
        gravity = glm::vec3(0.0f, -9.81f, 0.0f);
        onlyForward = false;
        casing = false;
        isJetpack = false;
    }
    else if (particleType == "jetpackUse"){
        texture = Texture2D::loadTextureFromFile("../../res/Particle/particle.png", true);
        amount = 1000;
        newParticles = 3;
        spawnDelay = 0.1f;
        speedVariation = 1.0f;
        XZvariation = 1.0f;
        particleLife = 4.0f;
        particleColor = glm::vec4(0.8f,0.8f,0.8f,1.0f);
        initialUpwardBoost = 0.0f;
        particleScale = 0.3f;
        gravity = glm::vec3(0.0f, -9.81f, 0.0f);
        onlyForward = false;
        casing = false;
        isJetpack = true;
    }
}

void ParticleGenerator::SpawnParticles() {
    generateParticle = true;
    hasSpawned = false;
}

void ParticleGenerator::SetInitialUpwardBoost(const glm::vec3& scale, float bonus) {
    // Assuming scale is in the range [0.5f, 2.0f]
    // Map the scale range to the initialUpwardBoost range [0.0f, 4.0f]
    float minScale = 0.5f;
    float maxScale = 2.0f;
    float minBoost = 0.0f;
    float maxBoost = 10.0f;

    // Calculate the ratio of the current scale value relative to the scale range
    float scaleRatio = (scale.x - minScale) / (maxScale - minScale);

    // Map the scale ratio to the initialUpwardBoost range
    initialUpwardBoost = minBoost + scaleRatio * (maxBoost - minBoost);

    // Set the initialUpwardBoost value
    initialUpwardBoost = glm::clamp(initialUpwardBoost, minBoost, maxBoost) + bonus;

    // Now, initialUpwardBoost ranges from 0.0f to 4.0f based on the scale
}

void ParticleGenerator::SetPartScale(const glm::vec3& scale, float bonus) {
    // Assuming scale is in the range [0.5f, 2.0f]
    // Map the scale range to the initialUpwardBoost range [0.0f, 4.0f]
    float minScale = 0.5f;
    float maxScale = 2.0f;
    float minBoost = 0.2f;
    float maxBoost = 0.9f;

    // Calculate the ratio of the current scale value relative to the scale range
    float scaleRatio = (scale.x - minScale) / (maxScale - minScale);

    // Map the scale ratio to the initialUpwardBoost range
    particleScale = minBoost + scaleRatio * (maxBoost - minBoost);

    // Set the initialUpwardBoost value
    particleScale = glm::clamp(particleScale, minBoost, maxBoost) + bonus;

    // Now, initialUpwardBoost ranges from 0.0f to 4.0f based on the scale
}