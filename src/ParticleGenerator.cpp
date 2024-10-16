#include "ParticleGenerator.h"
#include <glad/glad.h>
#include <vector>
#include <algorithm>

ParticleGenerator::ParticleGenerator(std::shared_ptr<Shader> shader, string particleType)
        : shader(shader), particleType(particleType) {
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(2);
}

ParticleGenerator::ParticleGenerator() : shader(nullptr), particleType("default") {
    _type = ComponentType::PARTICLEGENERATOR;
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(2);
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

    glm::quat rotationQuat = object->GetTransform()->GetRotation();
    glm::mat4 rotationMatrix = glm::toMat4(rotationQuat);

    rotatedOffset = glm::vec3(rotationMatrix * glm::vec4(offset, 1.0f));

    glm::quat objectRotation = object->GetTransform()->GetRotation();

    if(counterXrotation) {
        objectRotation.x = 0.0f;
        objectRotation.y = 0.0f;
        objectRotation.z = 0.0f;
    }


    computeShader->use();
    computeShader->setFloat("dt", dt);
    computeShader->setVec3("nodePosition", object->GetTransform()->GetPosition() + rotatedOffset);
    computeShader->setFloat("elapsedTime", elapsedTime);
    computeShader->setBool("shouldSpawn", shouldSpawn);
    computeShader->setVec3("cameraPosition", _cameraRef->GetPosition());
    computeShader->setVec3("cameraForward", _cameraRef->GetFrontVector());
    computeShader->setBool("hasSpawned", hasSpawned);
    computeShader->setMat4("objectRotation", glm::toMat4(objectRotation));
    computeShader->setVec3("enemyPosition", enemyPosition);
    computeShader->setVec3("jumpOff", jumpOffPoint);
    computeShader->setVec3("windDirection", WEATHERMANAGER.getWindDirection());
    computeShader->setFloat("windStrength", WEATHERMANAGER.getWindStrength());
    computeShader->setVec3("gravity", gravity);
    computeShader->setBool("tooltipSpawn", tooltipSpawn);
    computeShader->setBool("tooltipShrink", tooltipShrink);

    generatorPosition = object->GetTransform()->GetPosition() + rotatedOffset;

    hasSpawned = true;

    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleBuffer);
    glDispatchCompute(amount, 1, 1);
    glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
}

void ParticleGenerator::RenderParticles() {

    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleBuffer);
    void* bufferData = glMapBuffer(GL_SHADER_STORAGE_BUFFER, GL_WRITE_ONLY);
    if (bufferData) {
        std::memset(bufferData, 0, amount * sizeof(Particle));
        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);
    }
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);

    int deadParticles = 0;

    glm::vec3 objectPositionWithOffset = object->GetTransform()->GetPosition() + rotatedOffset;
    glm::vec3 vectorToObject = objectPositionWithOffset - _cameraRef->GetPosition();
    glm::vec3 normalizedCamForward = glm::normalize(_cameraRef->GetFrontVector());
    glm::vec3 normalizedVectorToObject = glm::normalize(vectorToObject);
    float dotProduct = glm::dot(normalizedCamForward, normalizedVectorToObject);
    bool visible = dotProduct > 0.1f;

    if((visible && particleType != "turretShot") || particleType == "turretShot" || particleType=="ambientSandParticles" || particleType=="rainParticles" || particleType =="rainParticlesFull" || particleType == "wormParticles" || particleType == "antSpawn") {

        Particle *particleData = this->particleData;

        std::vector<glm::vec3> offsets;
        std::vector<float> scales;

        for (unsigned int i = 0; i < amount; ++i) {
            Particle &particle = particleData[i];
            if (particle.Life > 0.0f) {
                offsets.push_back(particle.Position);
                scales.push_back(particle.Scale);
            } else {
                deadParticles++;
            }
        }

//        if (spawnDelay == 0.0f && threadCount >= 3) {
//            generateParticle = false;
//        }

        glUnmapBuffer(GL_SHADER_STORAGE_BUFFER);

        size_t offsetSize = offsets.size() * sizeof(glm::vec3);
        size_t scaleSize = scales.size() * sizeof(float);

        char *bufferPointer = instanceData[currentBuffer];
        memcpy(bufferPointer, offsets.data(), offsetSize);
        memcpy(bufferPointer + offsetSize, scales.data(), scaleSize);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[currentBuffer]);

        // Instance Position (Location 2)
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid *) 0);
        glVertexAttribDivisor(2, 1);

        // Instance Scale (Location 4)
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (GLvoid *) (offsetSize));
        glVertexAttribDivisor(4, 1);

        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        shader->use();
        shader->setVec4("pColor" ,particleColor);
        shader->setBool("showTexture", showTexture);
        shader->setBool("affectDirLight", affectDirLight);
        shader->setBool("plastic1", plastic1);
        shader->setBool("plastic2", plastic2);

        texture.Bind();

        glDrawArraysInstanced(GL_TRIANGLES, 0, 6, offsets.size());

        glBindVertexArray(0);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        currentBuffer = (currentBuffer + 1) % 3;
    }
    if(deadParticles == amount){
        threadCount++;
    }
    else{
        threadCount = 0;
    }

    if (threadCount >= 4) {
        if (toDelete) {
            generateParticle = false;
            GAMEMANAGER.root->RemoveChild(_ownerNode);
        }
        else if(particleType != "jetpackUse" && particleType != "flareParticles1" && particleType != "digPlastic" && particleType != "digMetal")
        {
            generateParticle = false;
        }
    }
}

void ParticleGenerator::Init() {
    computeShader = std::make_shared<ComputeShader>("res/particleCompute.glsl", "computeShader");
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
    computeShader->setBool("casing", casing);
    computeShader->setBool("isJetpack", isJetpack);
    computeShader->setFloat("groundLevel", GAMEMANAGER._groundLevel);
    computeShader->setBool("isFlare", isFlare);
    computeShader->setBool("ambient", ambient);
    computeShader->setBool("isUnderground", isUnderground);
    computeShader->setBool("rain", rain);
    computeShader->setBool("tooltip", tooltip);
    computeShader->setBool("worm", worm);

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

    size_t bufferSize = amount * sizeof(glm::vec3) + amount * sizeof(float);
    glGenBuffers(3, instanceVBOs);
    for (int i = 0; i < 3; ++i) {
        glBindBuffer(GL_ARRAY_BUFFER, instanceVBOs[i]);
        glBufferStorage(GL_ARRAY_BUFFER, bufferSize, nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
        instanceData[i] = (char*)glMapBufferRange(GL_ARRAY_BUFFER, 0, bufferSize, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    }

    // Attribute 2: Offsets
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
    glVertexAttribDivisor(2, 1);

    // Attribute 4: Scales
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 1, GL_FLOAT, GL_FALSE, sizeof(float), (GLvoid*)(amount * sizeof(glm::vec3) + amount * sizeof(glm::vec4)));
    glVertexAttribDivisor(4, 1);

    glBindVertexArray(0);

    particles.resize(amount);
    glGenBuffers(1, &particleBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, particleBuffer);
    glBufferStorage(GL_SHADER_STORAGE_BUFFER, amount * sizeof(Particle), nullptr, GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    particleData = (Particle*)glMapBufferRange(GL_SHADER_STORAGE_BUFFER, 0, amount * sizeof(Particle), GL_MAP_WRITE_BIT | GL_MAP_PERSISTENT_BIT | GL_MAP_COHERENT_BIT);
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, particleBuffer);
    glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
}

void ParticleGenerator::initiateParticleType() {
    if (particleType == "turretCasing") {
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 50;
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
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 30;
        newParticles = 1;
        spawnDelay = 0.0f;
        speedVariation = 0.2f;
        XZvariation = 90.0f;
        particleLife = 3.0f;
        particleColor = glm::vec4(0.8f,0.8f,0.0f,1.0f);
        initialUpwardBoost = 0.0f;
        particleScale = 0.25f;
        gravity = glm::vec3(0.0f, 0.0f, 0.0f);
        onlyForward = true;
        casing = false;
        isJetpack = false;
        affectDirLight = false;
    }
    else if (particleType == "antShot"){
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 500;
        newParticles = 4;
        spawnDelay = 0.0f;
        speedVariation = 1.2f;
        XZvariation = 2.0f;
        particleLife = 3.0f;
        particleColor = glm::vec4(0.4f,0.0f,0.0f,1.0f);
        SetInitialUpwardBoost(enemyScale, 4.0f);
        SetPartScale(enemyScale, 0.0f);
        gravity = glm::vec3(0.0f, -9.81f, 0.0f);
        onlyForward = false;
        casing = false;
        isJetpack = false;
    }
    else if (particleType == "antDie"){
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 70;
        newParticles = 70;
        spawnDelay = 0.0f;
        speedVariation = 6.5f;
        XZvariation = 4.0f;
        particleLife = 3.5f;
        particleColor = glm::vec4(0.762f,0.448f,0.143f,1.0f);
        initialUpwardBoost = 2.5f;
        SetPartScale(enemyScale, 0.1f);
        gravity = glm::vec3(0.0f, -9.81f, 0.0f);
        onlyForward = false;
        casing = false;
        isJetpack = false;
    }
    else if (particleType == "jetpackUse"){
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 100;
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
    else if (particleType == "flareParticles1"){
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 100;
        newParticles = 3;
        spawnDelay = 0.2f;
        speedVariation = 0.2f;
        XZvariation = 0.4f;
        particleLife = 8.0f;
        particleColor = glm::vec4(1.0f,0.123f,0.226f,1.0f);
        initialUpwardBoost = 0.50f;
        particleScale = 0.05f;
        gravity = glm::vec3(0.0f, 0.0f, 0.0f);
        onlyForward = false;
        casing = false;
        isJetpack = false;
        counterXrotation = true;
        isFlare = true;
        affectDirLight = false;
    }
    else if (particleType == "ambientSandParticles"){
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 200;
        newParticles = 200;
        spawnDelay = 0.0f;
        speedVariation = 0.2f;
        XZvariation = 100.4f;
        particleLife = 60.0f;
        particleColor = glm::vec4(0.882f,0.677f,0.351f,1.0f);
        initialUpwardBoost = 0.04f;
        particleScale = 0.15f;
        gravity = glm::vec3(0.0f, -0.0f, 0.0f);
        onlyForward = false;
        casing = false;
        isJetpack = false;
        counterXrotation = false;
        isFlare = false;
        ambient = true;
    }
    else if (particleType == "digMetal"){
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 30;
        newParticles = 3;
        spawnDelay = 0.0f;
        speedVariation = 1.0f;
        XZvariation = 3.0f;
        particleLife = 1.2f;
        particleColor = glm::vec4(0.6f,0.6f,0.6f,1.0f);
        initialUpwardBoost = 1.0f;
        particleScale = 0.27f;
        gravity = glm::vec3(0.0f, -9.81f, 0.0f);
        onlyForward = false;
        casing = false;
        isJetpack = false;
        counterXrotation = false;
        isFlare = false;
        ambient = false;
        isUnderground = true;
        affectDirLight = false;
        plastic1 = true;
    }
    else if (particleType == "digPlastic"){
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 30;
        newParticles = 3;
        spawnDelay = 0.0f;
        speedVariation = 1.0f;
        XZvariation = 3.0f;
        particleLife = 1.2f;
        particleColor = glm::vec4(0.8627f,0.0784f,0.2353f,1.0f);
        initialUpwardBoost = 1.0f;
        particleScale = 0.22f;
        gravity = glm::vec3(0.0f, -9.81f, 0.0f);
        onlyForward = false;
        casing = false;
        isJetpack = false;
        counterXrotation = false;
        isFlare = false;
        ambient = false;
        isUnderground = true;
        affectDirLight = false;
        plastic2 = true;
    }
    else if (particleType == "rainParticles"){
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 1000;
        newParticles = 2;
        spawnDelay = 0.0f;
        speedVariation = 0.2f;
        XZvariation = 100.4f;
        particleLife = 15.0f;
        particleColor = glm::vec4(0.482f,0.451f,0.882f,1.0f);
        initialUpwardBoost = 0.04f;
        particleScale = 0.13f;
        gravity = glm::vec3(0.0f, -0.0f, 0.0f);
        ambient = true;
        rain = true;
        onlyForward = false;
        casing = false;
        isJetpack = false;
        counterXrotation = false;
        isFlare = false;
        affectDirLight = false;
    }
    else if (particleType == "rainParticlesFull"){
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 4000;
        newParticles = 5;
        spawnDelay = 0.0f;
        speedVariation = 0.2f;
        XZvariation = 100.4f;
        particleLife = 15.0f;
        particleColor = glm::vec4(0.482f,0.451f,0.898f,1.0f);
        initialUpwardBoost = 0.04f;
        particleScale = 0.17f;
        gravity = glm::vec3(0.0f, -0.0f, 0.0f);
        ambient = true;
        rain = true;
        onlyForward = false;
        casing = false;
        isJetpack = false;
        counterXrotation = false;
        isFlare = false;
        affectDirLight = false;
    }
    else if (particleType == "tooltipParticle"){
        texture = Texture2D::loadTextureFromFile("res/Particle/particleTest.png", true);
        amount = 1;
        newParticles = 1;
        spawnDelay = 0.0f;
        speedVariation = 0.0f;
        XZvariation = 0.0f;
        particleLife = 10.0f;
        particleColor = glm::vec4(1.0f,1.0f,1.0f,1.0f);
        initialUpwardBoost = 0.04f;
        particleScale = 0.1f;
        gravity = glm::vec3(0.0f, 0.0f, 0.0f);
        tooltip = true;
        showTexture = true;
        affectDirLight = false;
    }
    else if (particleType == "waspDie"){
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 70;
        newParticles = 70;
        spawnDelay = 0.0f;
        speedVariation = 6.5f;
        XZvariation = 4.0f;
        particleLife = 3.5f;
        particleColor = glm::vec4(1.0f,0.6941f,0.1059f,1.0f);
        initialUpwardBoost = 2.5f;
        SetPartScale(enemyScale, 0.1f);
        gravity = glm::vec3(0.0f, -9.81f, 0.0f);
        onlyForward = false;
        casing = false;
        isJetpack = false;
    }
    else if (particleType == "beetleDie"){
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 150;
        newParticles = 150;
        spawnDelay = 0.0f;
        speedVariation = 7.5f;
        XZvariation = 4.0f;
        particleLife = 3.5f;
        particleColor = glm::vec4(0.0510f,0.1608f,0.2941f,1.0f);
        initialUpwardBoost = 2.5f;
        SetPartScale(enemyScale, 0.2f);
        gravity = glm::vec3(0.0f, -9.81f, 0.0f);
        onlyForward = false;
        casing = false;
        isJetpack = false;
    }
    else if (particleType == "antSpawn"){
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 20;
        newParticles = 20;
        spawnDelay = 0.0f;
        speedVariation = 20.5f;
        XZvariation = 2.0f;
        particleLife = 7.0f;
        particleColor = glm::vec4(0.782f,0.577f,0.251f,1.0f);
        initialUpwardBoost = 2.0f;
        SetPartScale(enemyScale, 0.8f);
        gravity = glm::vec3(0.0f, -3.0f, 0.0f);
        onlyForward = false;
        casing = false;
        isJetpack = false;
    }
    else if (particleType == "wormParticles"){
        texture = Texture2D::loadTextureFromFile("res/Particle/particle.png", true);
        amount = 700;
        newParticles = 3;
        spawnDelay = 0.0f;
        speedVariation = 50.5f;
        XZvariation = 40.5f;
        particleLife = 10.5f;
        particleColor = glm::vec4(0.832f,0.627f,0.301f,1.0f);
        initialUpwardBoost = -95.0f;
        particleScale = 1.6f;
        gravity = glm::vec3(0.0f, -9.8f, 0.0f);
        onlyForward = false;
        casing = false;
        isJetpack = false;
        worm = true;
    }
}

void ParticleGenerator::SpawnParticles() {
    threadCount = 0;
    generateParticle = true;
    hasSpawned = false;
}

void ParticleGenerator::SetInitialUpwardBoost(const glm::vec3& scale, float bonus) {
    // Assuming scale is in the range [0.5f, 2.0f]
    // Map the scale range to the initialUpwardBoost range [0.0f, 4.0f]
    float minScale = 0.5f;
    float maxScale = 2.0f;
    float minBoost = 0.0f;
    float maxBoost = 5.0f;

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
