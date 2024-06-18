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

    daySkyColor = glm::vec3(0.502f, 0.620f, 0.867f);
    sunsetSkyColor = glm::vec3(1.0f, 0.7059f, 0.4471f);
    nightSkyColor = glm::vec3(0.055f, 0.055f, 0.141f);
    rainDaySkyColor = glm::vec3(0.3686f, 0.4196f, 0.5255f);
    rainSunsetSkyColor = glm::vec3(0.6784f, 0.4824f, 0.3137f);
    rainNightSkyColor = glm::vec3(0.1294f, 0.1294f, 0.2196f);

    dayColor = glm::vec3(0.863f, 0.855f, 0.737f);
    sunsetColor = glm::vec3(0.9451f, 0.8314f, 0.7412f);
    nightColor = glm::vec3(0.1922f, 0.1922f, 0.3373f);
    rainDayColor = glm::vec3(0.4902f, 0.5098f, 0.7098f);
    rainSunsetColor = glm::vec3(0.4353f, 0.3686f, 0.3137f);
    rainNightColor = glm::vec3(0.1922f, 0.1922f, 0.3373f);
}

void WeatherManager::Update(){
    windDirection = windModel.getWind(15 * TIME.GetDeltaTime());
    windDirection.x /= 10;
    windDirection.y /= 10;
    windDirection.z /= 10;
    if(windDirection.y > 0.1) windDirection.y = 0.1;
    if(windDirection.y < -0.1) windDirection.y = -0.1;
    if(isRaining) windDirection.y = -2.0f;

    UpdateSunPosition();

    timeSinceLastCheck += TIME.GetDeltaTime();
    timer += TIME.GetDeltaTime();
    if(timeSinceLastCheck >= rainCheckInterval)
    {
        timeSinceLastCheck = 0.0f;

        if (isRaining && !thunder) {
            float chance = static_cast<float>(std::rand()) / RAND_MAX;
            if (chance < thunderProbability) {
                thunder = true;
                thunderTimeLeft = thunderDuration;
            }
        }
    }

    if(thunder){
        thunderTimeLeft -= TIME.GetDeltaTime();
        if (thunderTimeLeft <= 0.0f) {
            thunder = false;
        }
    }


    // Update rain duration
    if (isRaining) {
        rainTimeLeft -= TIME.GetDeltaTime();
        timeSinceLastParticleSpawn += TIME.GetDeltaTime();
        auto particleGenerator = NODESMANAGER.getNodeByName("RainParticles")->GetComponent<ParticleGenerator>();
        if (spawnedParticles < particleGenerator->amount) {
            if (rainTimeLeft >= (2 * rainDuration / 3) && timeSinceLastParticleSpawn >= particleSpawnInterval) {
                particleGenerator->gravity = glm::vec3(0.0f, -20.0f, 0.0f);
                particleGenerator->SpawnParticles();
                timeSinceLastParticleSpawn = 0.0f;
            } else if (rainTimeLeft < 2 * rainDuration / 3 && timeSinceLastParticleSpawn >= particleSpawnInterval) {
                NODESMANAGER.getNodeByName("RainParticles2")->GetComponent<ParticleGenerator>()->gravity = glm::vec3(0.0f, -30.0f, 0.0f);
                NODESMANAGER.getNodeByName("RainParticles2")->GetComponent<ParticleGenerator>()->SpawnParticles();
                timeSinceLastParticleSpawn = 0.0f;
            } else if (rainTimeLeft < rainDuration / 3 && timeSinceLastParticleSpawn >= particleSpawnInterval) {
                particleGenerator->gravity = glm::vec3(0.0f, -20.0f, 0.0f);
                particleGenerator->SpawnParticles();
                timeSinceLastParticleSpawn = 0.0f;
            }
        }
        if (rainTimeLeft <= 0.0f) {
            isRaining = false;
        }
    }

    if(counter < 2) {
        NODESMANAGER.getNodeByName("AmbientParticles")->GetComponent<ParticleGenerator>()->SpawnParticles();
        counter++;
    }
}

void WeatherManager::SetupAmbientParticles() {
    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), "AmbientParticles");
    auto node = NODESMANAGER.getNodeByName("AmbientParticles");
    node->GetTransform()->SetPosition(glm::vec3(50.0f,299.0f,50.0f));

    auto ambientParticles = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"), "ambientSandParticles");
    ambientParticles->SetOffset(glm::vec3(0.0f, 0.0f, 0.0f));
    ambientParticles->object = node;
    ambientParticles->Init();
    node->AddComponent(ambientParticles);
}

void WeatherManager::SetupRainParticles() {
    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), "RainParticles");
    auto node = NODESMANAGER.getNodeByName("RainParticles");
    node->GetTransform()->SetPosition(glm::vec3(49.0f,319.0f,49.0f));

    auto rainParticles = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"), "rainParticles");
    rainParticles->SetOffset(glm::vec3(0.0f, 0.5f, 0.0f));
    rainParticles->object = node;
    rainParticles->Init();
    node->AddComponent(rainParticles);

    NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), "RainParticles2");
    auto node2 = NODESMANAGER.getNodeByName("RainParticles2");
    node2->GetTransform()->SetPosition(glm::vec3(49.0f,319.0f,49.0f));

    auto rainParticles2 = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"), "rainParticlesFull");
    rainParticles2->SetOffset(glm::vec3(0.0f, 0.5f, 0.0f));
    rainParticles2->object = node2;
    rainParticles2->Init();
    node2->AddComponent(rainParticles2);
}

void WeatherManager::UpdateSunPosition() {
    timeAccumulator += TIME.GetDeltaTime();
    angle += speed * TIME.GetDeltaTime();

    if (timeAccumulator >= stepInterval) {
        // Calculate the step increment
        float step = speed * stepInterval;
        // Increment the angle
        if(dirAngle <= 46.9)
            dirAngle += step * direction;
        else{
            if(angle > dirAngle + 0.5) {
                direction = -1;
                dirAngle = 46.8;
            }
            else{
                direction = 0;
            }
        }

        // Reset the accumulator
        timeAccumulator -= stepInterval;
    }

    float sunX = LIGHTSMANAGER.lightCenter.x;
    float sunZ = LIGHTSMANAGER.lightCenter.z + radius * cos(angle);
    float sunY = LIGHTSMANAGER.lightCenter.y + radius* sin(angle);

    float sunXShader = LIGHTSMANAGER.lightCenter.x;
    float sunZShader = LIGHTSMANAGER.lightCenter.z + (radius + 40) * cos(angle);
    float sunYShader = LIGHTSMANAGER.lightCenter.y + (radius + 40) * sin(angle);

    float shadowSunX = LIGHTSMANAGER.shadowLightCenter.x;
    float shadowSunZ = LIGHTSMANAGER.shadowLightCenter.z + radius * cos(dirAngle);
    float shadowSunY = LIGHTSMANAGER.shadowLightCenter.y + radius* sin(dirAngle);

    if(angle > 45.5f && angle < 45.65){
        angle = 45.66;
        dirAngle = 45.66;
    }
    if(angle >= 48.5f){
        float randomChance = static_cast<float>(std::rand()) / (RAND_MAX + 1.0f);
        rainTimeLeft = rainDuration * randomChance + 0.7;
        angle -= 6.0f;
        if (randomChance < rainProbability){
            isRaining = true;
        }
        else
        {
            isRaining = false;
        }

        if(isRaining) rainyDay = true;
        if(!isRaining) rainyDay = false;
    }

    if(dirAngle < 44.3){
        direction = 0;
        if(angle > dirAngle) {
            dirAngle = angle;
            direction = 1;
        }
    }
    LIGHTSMANAGER.lightPos = glm::vec3(sunX,sunY,sunZ);
    LIGHTSMANAGER.lightPosShader = glm::vec3(sunXShader,sunYShader,sunZShader);
    LIGHTSMANAGER.shadowLightPos = glm::vec3(shadowSunX,shadowSunY,shadowSunZ);
}

glm::vec3 WeatherManager::getSkyColor() {
    glm::vec3 currentColor;

    if (angle >= 44.0f && angle < 45.5f) {
        // Sunrise to Day
        if(!rainyDay) currentColor = glm::mix(sunsetSkyColor, daySkyColor, (angle - 44.0f) / (45.5f - 44.0f));
        if(rainyDay) currentColor = glm::mix(rainSunsetSkyColor, rainDaySkyColor, (angle - 44.0f) / (45.5f - 44.0f));
    } else if(angle >= 45.5f && angle < 46.5){
        if(!rainyDay) currentColor = daySkyColor;
        if(rainyDay) currentColor = rainDaySkyColor;
    } else if (angle >= 46.5f && angle < 46.9f) {
        // Day to Sunset
        if(!rainyDay) currentColor = glm::mix(daySkyColor, sunsetSkyColor, (angle - 46.5f) / (46.9f - 46.5f));
        if(rainyDay) currentColor = glm::mix(rainDaySkyColor, rainSunsetSkyColor, (angle - 46.5f) / (46.9f - 46.5f));
    } else if (angle >= 46.9f && angle < 47.6f) {
        // Sunset to Night
        if(!rainyDay) currentColor = glm::mix(sunsetSkyColor, rainNightSkyColor, (angle - 46.9f) / (47.6f - 46.9f));
        if(rainyDay) currentColor = glm::mix(rainSunsetSkyColor, rainNightSkyColor, (angle - 46.9f) / (47.6f - 46.9f));
    } else if (angle >= 47.6 && angle < 49.0f){
        currentColor = rainNightSkyColor;
        speed = 0.4;
    } else if (angle >= 42.5 && angle < 44.0) {
        speed = 0.03;
        if(!rainyDay) currentColor = glm::mix(rainNightSkyColor, sunsetSkyColor, (angle - 42.5f) / (44.0f - 42.5f));
        if(rainyDay) currentColor = glm::mix(rainNightSkyColor, rainSunsetSkyColor, (angle - 42.5f) / (44.0f - 42.5f));
    }

    if(thunder) currentColor = glm::mix(currentColor, glm::vec3(0.9f,0.9f,0.8f), 0.8f);
    return currentColor;
}

glm::vec3 WeatherManager::getDirColor() {
    glm::vec3 currentColor;

    if (angle >= 44.0f && angle < 45.5f) {
        // Sunrise to Day
        if(!rainyDay) currentColor = glm::mix(sunsetColor, dayColor, (angle - 44.0f) / (45.5f - 44.0f));
        if(rainyDay) currentColor = glm::mix(rainSunsetColor, rainDayColor, (angle - 44.0f) / (45.5f - 44.0f));
    } else if(angle >= 45.5f && angle < 46.5){
        if(!rainyDay) currentColor = dayColor;
        if(rainyDay) currentColor = rainDayColor;
    } else if (angle >= 46.5f && angle < 46.9f) {
        // Day to Sunset
        if(!rainyDay) currentColor = glm::mix(dayColor, sunsetColor, (angle - 46.5f) / (46.9f - 46.5f));
        if(rainyDay) currentColor = glm::mix(rainDayColor, rainSunsetColor, (angle - 46.5f) / (46.9f - 46.5f));
    } else if (angle >= 46.9f && angle < 47.6f) {
        // Sunset to Night
        if(!rainyDay) currentColor = glm::mix(sunsetColor, rainNightColor, (angle - 46.9f) / (47.6f - 46.9f));
        if(rainyDay) currentColor = glm::mix(rainSunsetColor, rainNightColor, (angle - 46.9f) / (47.6f - 46.9f));
    } else if (angle >= 47.6 && angle < 49.0f){
        currentColor = rainNightColor;
    } else if (angle >= 42.5 && angle < 44.0) {
        if(!rainyDay) currentColor = glm::mix(rainNightColor, sunsetColor, (angle - 42.5f) / (44.0f - 42.5f));
        if(rainyDay) currentColor = glm::mix(rainNightColor, rainSunsetColor, (angle - 42.5f) / (44.0f - 42.5f));
    }

    if(thunder) currentColor = glm::mix(currentColor, glm::vec3(0.7f,0.7f,0.5f), 0.1f);
    return currentColor;
}