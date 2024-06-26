#include "WeatherManager.h"


WeatherManager &WeatherManager::getInstance() {
    static WeatherManager instance;
    return instance;
}

void WeatherManager::Init() {
    _playerNode = NODESMANAGER.getNodeByName("player");
    _soundNode = RESOURCEMANAGER.GetSoundByName("Rain");
    _thunderNode1 = RESOURCEMANAGER.GetSoundByName("piorun1");
    _thunderNode2 = RESOURCEMANAGER.GetSoundByName("piorun2");
    _thunderNode3 = RESOURCEMANAGER.GetSoundByName("piorun3");
    _flagNode = NODESMANAGER.getNodeByID(178);
    _flagRenderer = _flagNode->GetComponent<MeshRenderer>();
    _originalRotation = _flagNode->GetTransform()->GetRotation();

    _flag0 = RESOURCEMANAGER.GetModelByName("flagModel0");
    _flag1 = RESOURCEMANAGER.GetModelByName("flagModel1");
    _flag2 = RESOURCEMANAGER.GetModelByName("flagModel2");
    _flag3 = RESOURCEMANAGER.GetModelByName("flagModel3");
    _flag4 = RESOURCEMANAGER.GetModelByName("flagModel4");
    _flag5 = RESOURCEMANAGER.GetModelByName("flagModel5");
    _flag6 = RESOURCEMANAGER.GetModelByName("flagModel6");
    _flag7 = RESOURCEMANAGER.GetModelByName("flagModel7");
    _flag8 = RESOURCEMANAGER.GetModelByName("flagModel8");
    _flag9 = RESOURCEMANAGER.GetModelByName("flagModel9");
    _flag10 = RESOURCEMANAGER.GetModelByName("flagModel10");
    _flag11 = RESOURCEMANAGER.GetModelByName("flagModel11");

    windDirection = glm::vec3(0.0f,0.0f,0.0f);
    windStrength = 0.7f;
    windModel.initialize(0.1, 1.0, 0.1, 1.0, 1.0, 1.0, 2.0);

    SetupAmbientParticles();
    SetupRainParticles();

    daySkyColor = glm::vec3(0.502f, 0.620f, 0.867f);
    sunsetSkyColor = glm::vec3(1.0f, 0.7059f, 0.4471f);
    nightSkyColor = glm::vec3(0.055f, 0.055f, 0.141f);
    //nightSkyColor = glm::vec3(0.0863f, 0.0863f, 0.1725f);
    rainDaySkyColor = glm::vec3(0.3686f, 0.4196f, 0.5255f);
    rainSunsetSkyColor = glm::vec3(0.6784f, 0.4824f, 0.3137f);
    rainNightSkyColor = glm::vec3(0.1294f, 0.1294f, 0.2196f);
    //rainNightSkyColor = glm::vec3(0.1608f, 0.1608f, 0.2353f);

    dayColor = glm::vec3(0.863f, 0.855f, 0.737f);
    sunsetColor = glm::vec3(0.9451f, 0.8314f, 0.7412f);
    nightColor = glm::vec3(0.1722f, 0.1722f, 0.3173f);
    //nightColor = glm::vec3(0.0667f, 0.0667f, 0.0980f);
    rainDayColor = glm::vec3(0.4902f, 0.5098f, 0.7098f);
    rainSunsetColor = glm::vec3(0.4353f, 0.3686f, 0.3137f);
    rainNightColor = glm::vec3(0.1722f, 0.1722f, 0.3173f);
    //rainNightColor = glm::vec3(0.0667f, 0.0667f, 0.0980f);

    wormNode = NODESMANAGER.getNodeByID(166);

    SetupWormParticles();
}

void WeatherManager::Reset(){
    direction = 1;
    angle = 44.5f;
    dirAngle = 44.5f;
    SetupWormParticles();
}

void WeatherManager::Update(){

    if(wormNode->GetEnabled()) {
        wormParticleNode->GetComponent<ParticleGenerator>()->SpawnParticles();
        if (wormNode->GetTransform()->GetRotation().y > 0.1) {
            wormParticleNode->GetComponent<ParticleGenerator>()->toDelete = true;
        }
    }

    windDirection = windModel.getWind(15 * TIME.GetDeltaTime());
    windDirection.x /= 10;
    windDirection.y /= 10;
    windDirection.z /= 10;
    if(windDirection.y > 0.1) windDirection.y = 0.1;
    if(windDirection.y < -0.1) windDirection.y = -0.1;
    if(isRaining) windDirection.y = -2.0f;

    UpdateFlagRotation(0.1f);

    flagTimer += TIME.GetDeltaTime();

    if(flagTimer > 0.15f) {
        flagTimer = 0.0f;
        switch (flagAnimCount) {
            case 0:
                _actualFlag = _flag0;
                break;
            case 1:
                _actualFlag = _flag1;
                break;
            case 2:
                _actualFlag = _flag2;
                break;
            case 3:
                _actualFlag = _flag3;
                break;
            case 4:
                _actualFlag = _flag4;
                break;
            case 5:
                _actualFlag = _flag5;
                break;
            case 6:
                _actualFlag = _flag6;
                break;
            case 7:
                _actualFlag = _flag7;
                break;
            case 8:
                _actualFlag = _flag8;
                break;
            case 9:
                _actualFlag = _flag9;
                break;
            case 10:
                _actualFlag = _flag10;
                break;
            case 11:
                _actualFlag = _flag11;
                flagAnimCount = 0;
                break;
        }

        _flagRenderer->_model = _actualFlag;

        flagAnimCount++;
    }

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
                if(_playerNode->GetTransform()->GetPosition().y > 292) {
                    if (chance < 0.33) _thunderNode1->PlaySound(_playerNode);
                    else if (chance >= 0.33 && chance < 0.66) _thunderNode2->PlaySound(_playerNode);
                    else if (chance >= 0.66) _thunderNode3->PlaySound(_playerNode);
                }
            }
        }
    }

    if(thunder){
        thunderTimeLeft -= TIME.GetDeltaTime();
        if (thunderTimeLeft <= 0.0f) {
            thunder = false;
        }
    }

    if(isRaining && _playerNode->GetTransform()->GetPosition().y < 292 && !_soundNode->_isFadingAway && !_soundNode->_isRisingUp && _soundNode->GetVolume() != 0.0){
        _soundNode->_fadeAwayTarget = 0.0;
        _soundNode->FadeAway(1);
        cout << "Fade Away" << endl;
    }

    if(isRaining && _playerNode->GetTransform()->GetPosition().y >= 292 && !_soundNode->_isFadingAway && !_soundNode->_isRisingUp && _soundNode->GetVolume() == 0.0){
        if(_reachedTarget) _soundNode->_riseUpTarget = 0.4;
        else _soundNode->_riseUpTarget = 1.0;

        _soundNode->RiseUp(1, _playerNode);
        cout << _soundNode->_riseUpTarget << endl;
        cout << "Rise Up" << endl;
    }

    // Update rain duration
    if (isRaining) {
        cout << "TARGET: " << _soundNode->_riseUpTarget << endl;
        cout << "VOLUME: " << _soundNode->GetVolume() << endl;
        rainTimeLeft -= TIME.GetDeltaTime();
        timeSinceLastParticleSpawn += TIME.GetDeltaTime();
        auto particleGenerator = NODESMANAGER.getNodeByName("RainParticles")->GetComponent<ParticleGenerator>();
        if (spawnedParticles < particleGenerator->amount) {
            if (rainTimeLeft >= (2 * rainDuration / 3) && timeSinceLastParticleSpawn >= particleSpawnInterval) {
                particleGenerator->gravity = glm::vec3(0.0f, -20.0f, 0.0f);
                particleGenerator->SpawnParticles();
                timeSinceLastParticleSpawn = 0.0f;
                if(_adjustOnce){
                    _soundNode->_riseUpFrom = 0.0;
                    _soundNode->_riseUpTarget = 0.4;
                    if(_playerNode->GetTransform()->GetPosition().y > 292) _soundNode->RiseUp(5, _playerNode);
                    _adjustOnce = false;
                }
                if(_soundNode->GetVolume() >= 0.4) _reachedTarget = true;
            } else if (rainTimeLeft < 2 * rainDuration / 3 && rainTimeLeft >= rainDuration / 3 && timeSinceLastParticleSpawn >= particleSpawnInterval) {
                NODESMANAGER.getNodeByName("RainParticles2")->GetComponent<ParticleGenerator>()->gravity = glm::vec3(0.0f, -30.0f, 0.0f);
                NODESMANAGER.getNodeByName("RainParticles2")->GetComponent<ParticleGenerator>()->SpawnParticles();
                timeSinceLastParticleSpawn = 0.0f;
                if(!_adjustOnce){
                    _soundNode->_riseUpFrom = 0.4;
                    _soundNode->_riseUpTarget = 1.0;
                    if(_playerNode->GetTransform()->GetPosition().y > 292) _soundNode->RiseUp(5, _playerNode);
                    _adjustOnce = true;
                    _reachedTarget = false;
                }
            } else if (rainTimeLeft < rainDuration / 3 && timeSinceLastParticleSpawn >= particleSpawnInterval) {
                particleGenerator->gravity = glm::vec3(0.0f, -20.0f, 0.0f);
                particleGenerator->SpawnParticles();
                timeSinceLastParticleSpawn = 0.0f;
                if(_adjustOnce){
                    _soundNode->_fadeAwayTarget = 0.4;
                    if(_playerNode->GetTransform()->GetPosition().y > 292) _soundNode->FadeAway(5);
                    _adjustOnce = false;
                }
                if(_soundNode->GetVolume() <= 0.4) _reachedTarget = true;
            }
        }
        if (rainTimeLeft <= 0.0f) {
            isRaining = false;
            _soundNode->_fadeAwayTarget = 0.0;
            if(_playerNode->GetTransform()->GetPosition().y > 292) _soundNode->FadeAway(5);
            _adjustOnce = true;
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
    float sunZShader = LIGHTSMANAGER.lightCenter.z + (radius + 80) * cos(angle);
    float sunYShader = LIGHTSMANAGER.lightCenter.y + (radius + 80) * sin(angle);

    float shadowSunX = LIGHTSMANAGER.shadowLightCenter.x;
    float shadowSunZ = LIGHTSMANAGER.shadowLightCenter.z + radius * cos(dirAngle);
    float shadowSunY = LIGHTSMANAGER.shadowLightCenter.y + radius* sin(dirAngle);

    if(angle > 45.5f && angle < 45.65){
        angle = 45.66;
        dirAngle = 45.66;
    }
    if(angle >= 48.5f){
        float randomChance = static_cast<float>(std::rand()) / (RAND_MAX + 1.0f);
        rainTimeLeft = rainDuration * (randomChance + 0.5);
        angle -= 6.0f;
        if (randomChance < rainProbability){
            isRaining = true;
            rainProbability = 0.1;
        }
        else
        {
            isRaining = false;
        }

        if(isRaining) rainyDay = true;
        if(!isRaining) {
            rainyDay = false;
        }

        if(_isDisco) {
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis_real(0.0, 1.0);

            daySkyColor = glm::vec3(dis_real(gen), dis_real(gen), dis_real(gen));
            sunsetSkyColor = glm::vec3(dis_real(gen), dis_real(gen), dis_real(gen));
            nightSkyColor = glm::vec3(dis_real(gen), dis_real(gen), dis_real(gen));
            rainDaySkyColor = glm::vec3(dis_real(gen), dis_real(gen), dis_real(gen));
            rainSunsetSkyColor = glm::vec3(dis_real(gen), dis_real(gen), dis_real(gen));
            rainNightSkyColor = glm::vec3(dis_real(gen), dis_real(gen), dis_real(gen));
        }

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
        speed = 0.1;
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

void WeatherManager::SetupWormParticles() {
    wormParticleNode = NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), "wormParticleNode");
    auto wormParticles = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"), "wormParticles");
    wormParticles->SetOffset(glm::vec3(-233.1878f, -102.0f, 0.0f));
    wormParticles->object = wormNode;
    wormParticles->Init();
    wormParticleNode->AddComponent(wormParticles);
}

void WeatherManager::MakeDisco(){
    _isDisco = !_isDisco;

    if(_isDisco) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis_real(0.0, 1.0);

        daySkyColor = glm::vec3(dis_real(gen), dis_real(gen), dis_real(gen));
        sunsetSkyColor = glm::vec3(dis_real(gen), dis_real(gen), dis_real(gen));
        nightSkyColor = glm::vec3(dis_real(gen), dis_real(gen), dis_real(gen));
        rainDaySkyColor = glm::vec3(dis_real(gen), dis_real(gen), dis_real(gen));
        rainSunsetSkyColor = glm::vec3(dis_real(gen), dis_real(gen), dis_real(gen));
        rainNightSkyColor = glm::vec3(dis_real(gen), dis_real(gen), dis_real(gen));
    }
    else{
        daySkyColor = glm::vec3(0.502f, 0.620f, 0.867f);
        sunsetSkyColor = glm::vec3(1.0f, 0.7059f, 0.4471f);
        nightSkyColor = glm::vec3(0.055f, 0.055f, 0.141f);
        rainDaySkyColor = glm::vec3(0.3686f, 0.4196f, 0.5255f);
        rainSunsetSkyColor = glm::vec3(0.6784f, 0.4824f, 0.3137f);
        rainNightSkyColor = glm::vec3(0.1294f, 0.1294f, 0.2196f);
    }
}

void WeatherManager::UpdateFlagRotation(float lerpValue){
    // Calculate the direction in the XZ plane
    glm::vec3 directionXZ = glm::vec3(windDirection.x, 0.0f, windDirection.z);
    directionXZ = glm::normalize(directionXZ);

    // Calculate the angle between the forward vector and the directionXZ
    glm::vec3 forward = glm::vec3(-1.0f, 0.0f, 0.0f);
    float cosTheta = glm::dot(forward, directionXZ);
    float angle = glm::acos(glm::clamp(cosTheta, -1.0f, 1.0f));
    if (directionXZ.x < 0.0f) {
        angle = -angle;
    }

    // Calculate the target rotation quaternion
    glm::quat targetRotation = glm::angleAxis(angle, glm::vec3(0.0f, 1.0f, 0.0f));
    glm::quat newRotation = _originalRotation * targetRotation;

    // Interpolate between the current rotation and the new rotation
    glm::quat currentRotation = _flagNode->GetTransform()->GetRotation();
    glm::quat smoothedRotation = glm::slerp(currentRotation, newRotation, lerpValue * TIME.GetDeltaTime());

    // Update the flag's rotation
    _flagNode->GetTransform()->SetRotation(smoothedRotation);
}