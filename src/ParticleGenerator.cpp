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
}

void ParticleGenerator::Init() {
    // Initialize compute shader and set its parameters
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
    computeShader->setBool("onlyForward", onlyForward);
    computeShader->setVec3("gravity", gravity);

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
    if (particleType == "antWalk") {
        texture = Texture2D::loadTextureFromFile("../../res/Particle/particle.png", true);
        amount = 1000;
        newParticles = 100;
        spawnDelay = 0.0f;
        speedVariation = 0.2f;
        XZvariation = 30.5f;
        particleLife = 4.0f;
        particleColor = glm::vec4(1.0f,0.0f,0.0f,1.0f);
        initialUpwardBoost = 3.0f;
        particleScale = 0.3f;
        gravity = glm::vec3(0.0f, -9.81f, 0.0f);
        onlyForward = false;

    }
    else if (particleType == "turretShot"){
        texture = Texture2D::loadTextureFromFile("../../res/Particle/particle.png", true);
        amount = 100;
        newParticles = 1;
        spawnDelay = 0.2f;
        speedVariation = 0.2f;
        XZvariation = 50.5f;
        particleLife = 4.0f;
        particleColor = glm::vec4(1.0f,0.0f,0.0f,1.0f);
        initialUpwardBoost = 3.0f;
        particleScale = 0.3f;
        gravity = glm::vec3(0.0f, -0.2f, 0.0f);
        onlyForward = true;
    }
}

void ParticleGenerator::SpawnParticles() {
    generateParticle = true;
    hasSpawned = false;
}

void ParticleGenerator::StopParticles() {
    generateParticle = false;
}