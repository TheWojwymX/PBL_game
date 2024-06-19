#include "LightsManager.h"


LightsManager &LightsManager::getInstance() {
    static LightsManager instance;
    return instance;
}

void LightsManager::Update() {
    flashlightCurrentPosition = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->LerpPosition(flashlightCurrentPosition);
    flashlightCurrentDirection = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->LerpDirection(flashlightCurrentDirection);

    skyColor = WEATHERMANAGER.getSkyColor();
    dirColor = WEATHERMANAGER.getDirColor();

    UpdateShaders();
    UpdateGlowsticks();
    UpdateActiveShots();
    UpdateLights();
}

void LightsManager::Reset()
{
    _oldAmountOfGlowsticks = glowstickCount;
    TurnOffGlowsticks();
    for (int i = 0; i < _glowsticksNodes.size(); i++) {
        if (_glowsticksNodes[i] == nullptr) continue;
        NODESMANAGER.getNodeByName("root")->RemoveChild(_glowsticksNodes[i]);
    }
}

void LightsManager::InitLights() {
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(2);

    instancedSandShader = RESOURCEMANAGER.GetShaderByName("instancedSandShader");
    instancedMetalShader = RESOURCEMANAGER.GetShaderByName("instancedMetalShader");
    instancedPlasticShader = RESOURCEMANAGER.GetShaderByName("instancedPlasticShader");
    modelShader = RESOURCEMANAGER.GetShaderByName("modelShader");
    shovelShader = RESOURCEMANAGER.GetShaderByName("shovelShader");
    glowstickShader = RESOURCEMANAGER.GetShaderByName("glowstickShader");
    outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    lightObjectShader = RESOURCEMANAGER.GetShaderByName("lightObjectShader");
    cloudShader = RESOURCEMANAGER.GetShaderByName("cloudShader");
    particleShader = RESOURCEMANAGER.GetShaderByName("particleShader");

    InitShadersForPointLights();

    //Flashlight Properties
    flashlightCurrentPosition = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition();
    flashlightCurrentDirection = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetFrontVector();

    lightPos = glm::vec3(49.999f, 330.0f, 120.0f);
    lightCenter = glm::vec3(45.0f, 300.0f,45.0f);

    lightPosShader = glm::vec3(49.999f, 330.0f, 120.0f);

    shadowLightPos = glm::vec3(49.999f, 330.0f, 120.0f);
    shadowLightCenter = glm::vec3(45.0f, 300.0f,45.0f);
}

void LightsManager::UpdateShaders(){
    modelShader->use();
    //modelShader->setBool("spotLights[0].isActive", isSpotActive);
    modelShader->setVec3("spotLights[0].position", flashlightCurrentPosition);
    modelShader->setVec3("spotLights[0].direction", flashlightCurrentDirection);
    modelShader->setFloat("spotLights[0].constant", flashlightConstant);
    modelShader->setFloat("spotLights[0].linear", flashlightLinear);
    modelShader->setFloat("spotLights[0].quadratic", flashlightQuadratic);
    modelShader->setVec3("spotLights[0].color", flashlightColor);
    modelShader->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(flashlightCutOff)));
    modelShader->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(flashlightOuterCutOff)));

    modelShader->setVec3("dirLight.direction", dirDirection);
    modelShader->setVec3("dirLight.color", dirColor);
    modelShader->setInt("dirLight.isActive", dirActive);
    modelShader->setVec3("lightPos", lightPos);

    glowstickShader->use();
    //glowstickShader->setBool("spotLights[0].isActive", isSpotActive);
    glowstickShader->setVec3("spotLights[0].position", flashlightCurrentPosition);
    glowstickShader->setVec3("spotLights[0].direction", flashlightCurrentDirection);
    glowstickShader->setFloat("spotLights[0].constant", flashlightConstant);
    glowstickShader->setFloat("spotLights[0].linear", flashlightLinear);
    glowstickShader->setFloat("spotLights[0].quadratic", flashlightQuadratic);
    glowstickShader->setVec3("spotLights[0].color", flashlightColor);
    glowstickShader->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(flashlightCutOff)));
    glowstickShader->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(flashlightOuterCutOff)));

    glowstickShader->setVec3("dirLight.direction", dirDirection);
    glowstickShader->setVec3("dirLight.color", dirColor);
    glowstickShader->setInt("dirLight.isActive", dirActive);
    glowstickShader->setVec3("lightPos", lightPos);

    outlineShader->use();
    outlineShader->setVec3("dirLight.direction", dirDirection);
    outlineShader->setVec3("dirLight.color", dirColor);
    outlineShader->setInt("dirLight.isActive", dirActive);

    instancedSandShader->use();
    //instancedSandShader->setBool("spotLights[0].isActive", isSpotActive);
    instancedSandShader->setVec3("spotLights[0].position", flashlightCurrentPosition);
    instancedSandShader->setVec3("spotLights[0].direction", flashlightCurrentDirection);
    instancedSandShader->setFloat("spotLights[0].constant", flashlightConstant);
    instancedSandShader->setFloat("spotLights[0].linear", flashlightLinear);
    instancedSandShader->setFloat("spotLights[0].quadratic", flashlightQuadratic);
    instancedSandShader->setVec3("spotLights[0].color", flashlightColor);
    instancedSandShader->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(flashlightCutOff)));
    instancedSandShader->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(flashlightOuterCutOff)));

    instancedSandShader->setVec3("dirLight.direction", dirDirection);
    instancedSandShader->setVec3("dirLight.color", dirColor);
    instancedSandShader->setInt("dirLight.isActive", dirActive);
    instancedSandShader->setVec3("lightPos", lightPos);

    instancedMetalShader->use();
    //instancedMetalShader->setBool("spotLights[0].isActive", isSpotActive);
    instancedMetalShader->setVec3("spotLights[0].position", flashlightCurrentPosition);
    instancedMetalShader->setVec3("spotLights[0].direction", flashlightCurrentDirection);
    instancedMetalShader->setFloat("spotLights[0].constant", flashlightConstant);
    instancedMetalShader->setFloat("spotLights[0].linear", flashlightLinear);
    instancedMetalShader->setFloat("spotLights[0].quadratic", flashlightQuadratic);
    instancedMetalShader->setVec3("spotLights[0].color", flashlightColor);
    instancedMetalShader->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(flashlightCutOff)));
    instancedMetalShader->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(flashlightOuterCutOff)));

    instancedMetalShader->setVec3("dirLight.direction", dirDirection);
    instancedMetalShader->setVec3("dirLight.color", dirColor);
    instancedMetalShader->setInt("dirLight.isActive", dirActive);
    instancedMetalShader->setVec3("lightPos", lightPos);

    instancedPlasticShader->use();
    //instancedPlasticShader->setBool("spotLights[0].isActive", isSpotActive);
    instancedPlasticShader->setVec3("spotLights[0].position", flashlightCurrentPosition);
    instancedPlasticShader->setVec3("spotLights[0].direction", flashlightCurrentDirection);
    instancedPlasticShader->setFloat("spotLights[0].constant", flashlightConstant);
    instancedPlasticShader->setFloat("spotLights[0].linear", flashlightLinear);
    instancedPlasticShader->setFloat("spotLights[0].quadratic", flashlightQuadratic);
    instancedPlasticShader->setVec3("spotLights[0].color", flashlightColor);
    instancedPlasticShader->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(flashlightCutOff)));
    instancedPlasticShader->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(flashlightOuterCutOff)));

    instancedPlasticShader->setVec3("dirLight.direction", dirDirection);
    instancedPlasticShader->setVec3("dirLight.color", dirColor);
    instancedPlasticShader->setInt("dirLight.isActive", dirActive);
    instancedPlasticShader->setVec3("lightPos", lightPos);

    for (int i = 0; i < maxLights; i++) {
        string name = "pointLights[" + to_string(i) + "]";
        instancedSandShader->use();
        instancedSandShader->setFloat(name + ".constant", glowstickConstant);
        instancedSandShader->setFloat(name + ".linear", glowstickLinear);
        instancedSandShader->setFloat(name + ".quadratic", glowstickQuadratic);

        instancedMetalShader->use();
        instancedMetalShader->setFloat(name + ".constant", glowstickConstant);
        instancedMetalShader->setFloat(name + ".linear", glowstickLinear);
        instancedMetalShader->setFloat(name + ".quadratic", glowstickQuadratic);

        instancedPlasticShader->use();
        instancedPlasticShader->setFloat(name + ".constant", glowstickConstant);
        instancedPlasticShader->setFloat(name + ".linear", glowstickLinear);
        instancedPlasticShader->setFloat(name + ".quadratic", glowstickQuadratic);

        shovelShader->use();
        shovelShader->setFloat(name + ".constant", glowstickConstant);
        shovelShader->setFloat(name + ".linear", glowstickLinear);
        shovelShader->setFloat(name + ".quadratic", glowstickQuadratic);

        modelShader->use();
        modelShader->setFloat(name + ".constant", glowstickConstant);
        modelShader->setFloat(name + ".linear", glowstickLinear);
        modelShader->setFloat(name + ".quadratic", glowstickQuadratic);
    }

    shovelShader->use();
    //shovelShader->setBool("spotLights[0].isActive", isSpotActive);
    shovelShader->setVec3("spotLights[0].position", flashlightCurrentPosition);
    shovelShader->setVec3("spotLights[0].direction", flashlightCurrentDirection);
    shovelShader->setFloat("spotLights[0].constant", flashlightConstant);
    shovelShader->setFloat("spotLights[0].linear", flashlightLinear);
    shovelShader->setFloat("spotLights[0].quadratic", flashlightQuadratic);
    shovelShader->setVec3("spotLights[0].color", flashlightColor);
    shovelShader->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(flashlightCutOff)));
    shovelShader->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(flashlightOuterCutOff)));

    shovelShader->setVec3("dirLight.direction", dirDirection);
    shovelShader->setVec3("dirLight.color", dirColor);
    shovelShader->setInt("dirLight.isActive", dirActive);
    shovelShader->setVec3("lightPos", lightPos);

    lightObjectShader->use();
    lightObjectShader->setVec3("lightColor", dirColor);

    cloudShader->use();
    cloudShader->setVec3("dirLight.direction", dirDirection);
    cloudShader->setVec3("dirLight.color", dirColor);
    cloudShader->setInt("dirLight.isActive", dirActive);

    particleShader->use();
    particleShader->setVec3("dirColor", dirColor);
}

void LightsManager::AddGlowstick() {
    glowstickCount++;
    std::string glowstickNodeName = "Glowstick" + to_string(glowstickCount);
    shared_ptr<Node> glowstickNode = NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), glowstickNodeName);
    _glowsticksNodes.push_back(glowstickNode);
    glowstickNode->GetTransform()->SetPosition(ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
    glowstickNode->GetTransform()->SetScale(glm::vec3(1.4, 1.4, 1.4));

    auto glowstickMeshRenderer = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    glowstickMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("glowstickModel");
    glowstickMeshRenderer->_shader = RESOURCEMANAGER.GetShaderByName("glowstickShader");
    glowstickMeshRenderer->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    glowstickMeshRenderer->Initiate();
    glowstickNode->AddComponent(glowstickMeshRenderer);

    auto glowstickMovement = COMPONENTSMANAGER.CreateComponent<GlowstickMovement>();
    glowstickNode->AddComponent(glowstickMovement);
    glowstickMovement->Init();

    glowstickColors.push_back(GenerateRandomColor());

    glowstickShader->use();
    glowstickShader->setVec3("glowstickColor", glowstickColor);
}

void LightsManager::AddShot(const glm::vec3& startPos, const glm::vec3& endPos) {
    _activeShots.emplace_back(startPos, endPos, glm::vec3(0.9568f, 0.9450f, 0.5254f));//
}

void LightsManager::UpdateGlowsticks() {
    glm::vec3 camPosition = _cameraRef->GetPosition();
    _visibleGlowsticks.clear();
    // Iterate over all glowstick nodes
    for (const auto& glowstickNode : _glowsticksNodes) {
        // Get the transform component of the node
        auto transform = glowstickNode->GetTransform();

        // Get the position of the glowstick
        glm::vec3 glowstickPosition = transform->GetPosition();

        // Calculate distance to camera
        float distToCamera = glm::distance(glowstickPosition, camPosition);

        // Check if within max distance
        if (distToCamera < maxDistance) {
            // Get the index of the glowstick node in _glowsticksNodes
            int index = std::distance(_glowsticksNodes.begin(), std::find(_glowsticksNodes.begin(), _glowsticksNodes.end(), glowstickNode));

            // Ensure index is valid for accessing glowstickColors
            if (index >= 0 && index < glowstickColors.size()) {
                // Add visible glowstick to _visibleGlowsticks
                _visibleGlowsticks.emplace_back(glowstickPosition, glowstickColors[index], distToCamera, LightSource::GLOWSTICK);
            }
        }
    }

    // Sort visible glowsticks
    SortVisibleLights(_visibleGlowsticks);
}


void LightsManager::TurnOffGlowsticks() {
    for(int i = 0; i < maxLights; i++) {
        string name = "pointLights[" + to_string(i) + "]";
        instancedSandShader->use();
        instancedSandShader->setBool(name + ".isActive", false);

        instancedMetalShader->use();
        instancedMetalShader->setBool(name + ".isActive", false);

        instancedPlasticShader->use();
        instancedPlasticShader->setBool(name + ".isActive", false);

        shovelShader->use();
        shovelShader->setBool(name + ".isActive", false);
    }

    for(int i = 0; i < glowstickCount; i++) {
        string name = "pointLights[" + to_string(i) + "]";
        auto thisGlowstick = NODESMANAGER.getNodeByName("Glowstick" + to_string(i + 1));
        if(thisGlowstick != nullptr) {
            GAMEMANAGER.root->RemoveChild(thisGlowstick);
        }
    }
}

void LightsManager::UpdateActiveShots() {
    // Iterate over all active shots
    for (auto it = _activeShots.begin(); it != _activeShots.end();) {
        ActiveShot& shot = *it;

        // Accumulate elapsed time
        shot.elapsedTime += TIME.GetDeltaTime();

        // Calculate the direction towards the end position
        glm::vec3 direction = glm::normalize(shot.endPos - shot.startPos);

        // Calculate the distance to move this update
        float distance = glm::length(shot.endPos - shot.startPos);
        float step = _bulletSpeed * shot.elapsedTime; // Use accumulated time

        // Check if we can reach the end position in this step
        if (step >= distance) {
            // Set the start position to the end position
            shot.startPos = shot.endPos;
            // Remove the shot from the list
            it = _activeShots.erase(it);
        }
        else {
            // Update the start position using LERP
            float t = step / distance;
            shot.startPos = glm::mix(shot.startPos, shot.endPos, t);
            ++it;
        }
    }

    SortActiveShots();
}

void LightsManager::SortActiveShots() {
    glm::vec3 camPosition = _cameraRef->GetPosition();

    // Clear _visibleShots and populate with shots within maxDistance
    _visibleShots.clear();
    for (const auto& shot : _activeShots) {
        float distToCamera = glm::distance(shot.startPos, camPosition);
        if (distToCamera <= maxDistance) {
            _visibleShots.emplace_back(shot.startPos, shot.color, distToCamera, LightSource::SHOT);
        }
    }

    SortVisibleLights(_visibleShots);
}

void LightsManager::UpdateLights() {

    std::vector<VisibleLight> combinedLights;

    combinedLights.reserve(_visibleGlowsticks.size() + _visibleShots.size());

    // Insert elements from _visibleGlowsticks
    combinedLights.insert(combinedLights.end(), _visibleGlowsticks.begin(), _visibleGlowsticks.end());

    // Insert elements from _visibleShots
    combinedLights.insert(combinedLights.end(), _visibleShots.begin(), _visibleShots.end());

    SortVisibleLights(combinedLights);

    UpdateShadersPointLights(combinedLights);
}

void LightsManager::InitShadersForPointLights()
{
    // Add shaders to the vector
    _shadersForPointLights.push_back(instancedSandShader);
    _shadersForPointLights.push_back(instancedMetalShader);
    _shadersForPointLights.push_back(instancedPlasticShader);
    _shadersForPointLights.push_back(modelShader);
    _shadersForPointLights.push_back(shovelShader);
}

void LightsManager::SortVisibleLights(std::vector<VisibleLight>& lights) {
    // Sort lights based on distance to the camera
    std::sort(lights.begin(), lights.end(),
        [](const VisibleLight& a, const VisibleLight& b) {
            return a.distance < b.distance;
        });
}

void LightsManager::UpdateShadersPointLights(const std::vector<VisibleLight>& pointLights)
{
    // Ensure maxLights does not exceed the available lights
    int lightsAmount = std::min(maxLights, static_cast<int>(pointLights.size()));

    // Update shaders for the first maxLights lights
    for (int i = 0; i < lightsAmount; ++i) {
        std::string name = "pointLights[" + std::to_string(i) + "]";
        const VisibleLight& light = pointLights[i];

        for (const auto& shader : _shadersForPointLights) {
            UpdateShaderPointLight(shader, name, light.pos,light.color,light.sourceType);
        }
    }

    // Deactivate lights beyond maxLights
    for (int i = lightsAmount; i < maxLights; ++i) {
        std::string name = "pointLights[" + std::to_string(i) + "].isActive";
        for (const auto& shader : _shadersForPointLights) {
            shader->use();
            shader->setBool(name, false);
        }
    }
}

void LightsManager::UpdateShaderPointLight(std::shared_ptr<Shader> shader, std::string name, glm::vec3 pos, glm::vec3 color, LightSource lightSource)
{
    shader->use();
    shader->setBool(name + ".isActive", true);
    shader->setVec3(name + ".position", pos);
    shader->setVec3(name + ".color", color);

    switch (lightSource) {
        case LightSource::GLOWSTICK:
            shader->setFloat(name + ".constant", glowstickConstant);
            shader->setFloat(name + ".linear", glowstickLinear);
            shader->setFloat(name + ".quadratic", glowstickQuadratic);
            shader->setBool(name + ".isShot", false);
            break;
        case LightSource::SHOT:
            shader->setFloat(name + ".constant", shotConstant);
            shader->setFloat(name + ".linear", shotLinear);
            shader->setFloat(name + ".quadratic", shotQuadratic);
            shader->setBool(name + ".isShot", true);
            break;
        default:
            break;
    }
}

glm::vec3 LightsManager::GenerateRandomColor()
{
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    do {
        glowstickColor = glm::vec3(dis(gen), dis(gen), dis(gen));
    } while (glowstickColor.x <= 0.5f && glowstickColor.y <= 0.5f && glowstickColor.z <= 0.7f);

    return glowstickColor;
}
