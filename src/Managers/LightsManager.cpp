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
}

void LightsManager::Reset()
{
    _oldAmountOfGlovsticks = glowstickCount;
    TurnOffGlowsticks();
    for (int i = 0; i < _glowsticksNodes.size(); i++) {
        if (_glowsticksNodes[i] == nullptr) continue;
        NODESMANAGER.getNodeByName("root")->RemoveChild(_glowsticksNodes[i]);
    }
}

void LightsManager::InitLights() {
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

    //Flashlight Properties
    isSpotActive = false;
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

    for (int i = 0; i < maxGlowsticks; i++) {
        string name = "pointLights[" + to_string(i) + "]";
        if(isSpotActive) {
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
        }
        else{
            instancedSandShader->use();
            instancedSandShader->setFloat(name + ".constant", glowstickConstantNoFlash);
            instancedSandShader->setFloat(name + ".linear", glowstickLinearNoFlash);
            instancedSandShader->setFloat(name + ".quadratic", glowstickQuadraticNoFlash);

            instancedMetalShader->use();
            instancedMetalShader->setFloat(name + ".constant", glowstickConstantNoFlash);
            instancedMetalShader->setFloat(name + ".linear", glowstickLinearNoFlash);
            instancedMetalShader->setFloat(name + ".quadratic", glowstickQuadraticNoFlash);

            instancedPlasticShader->use();
            instancedPlasticShader->setFloat(name + ".constant", glowstickConstantNoFlash);
            instancedPlasticShader->setFloat(name + ".linear", glowstickLinearNoFlash);
            instancedPlasticShader->setFloat(name + ".quadratic", glowstickQuadraticNoFlash);

            shovelShader->use();
            shovelShader->setFloat(name + ".constant", glowstickConstant);
            shovelShader->setFloat(name + ".linear", glowstickLinear);
            shovelShader->setFloat(name + ".quadratic", glowstickQuadratic);
        }
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

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-1.0f, 1.0f);

    do {
        glowstickColor = glm::vec3(dis(gen), dis(gen), dis(gen));
    } while (glowstickColor.x <= 0.5f && glowstickColor.y <= 0.5f && glowstickColor.z <= 0.7f);
    glowstickColors.push_back(glowstickColor);

    glowstickShader->use();
    glowstickShader->setVec3("glowstickColor", glowstickColor);
}

void LightsManager::UpdateGlowsticks() {
    glm::vec3 camPosition = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition();
    std::vector<glm::vec3> visibleGlowsticks;
    std::vector<glm::vec3> visibleGlowstickColors;

    for (int i = _oldAmountOfGlovsticks; i < glowstickCount; i++) {

        auto thisGlowstick = NODESMANAGER.getNodeByName("Glowstick" + to_string(i + 1));
        if(thisGlowstick != nullptr) {
            glm::vec3 glowstickPosition = thisGlowstick->GetTransform()->GetPosition();

            if (glm::distance(glowstickPosition, camPosition) < 60.0f) {
                visibleGlowsticks.push_back(glowstickPosition);
                visibleGlowstickColors.push_back(glowstickColors[i]);
            }
        }
    }

    std::vector<std::pair<glm::vec3, glm::vec3>> combined;
    for (size_t i = 0; i < visibleGlowsticks.size(); ++i) {
        combined.emplace_back(visibleGlowsticks[i], visibleGlowstickColors[i]);
    }

    std::sort(combined.begin(), combined.end(), [&](const std::pair<glm::vec3, glm::vec3>& a, const std::pair<glm::vec3, glm::vec3>& b) {
        return glm::distance(a.first, camPosition) < glm::distance(b.first, camPosition);
    });

    for (size_t i = 0; i < combined.size(); ++i) {
        visibleGlowsticks[i] = combined[i].first;
        visibleGlowstickColors[i] = combined[i].second;
    }

    for (int i = 0; i < visibleGlowsticks.size(); i++) {
        string name = "pointLights[" + to_string(i) + "]";
        instancedSandShader->use();
        instancedSandShader->setBool(name + ".isActive", true);
        instancedSandShader->setVec3(name + ".position", visibleGlowsticks[i]);
        instancedSandShader->setVec3(name + ".color", visibleGlowstickColors[i]);

        instancedMetalShader->use();
        instancedMetalShader->setBool(name + ".isActive", true);
        instancedMetalShader->setVec3(name + ".position", visibleGlowsticks[i]);
        instancedMetalShader->setVec3(name + ".color", visibleGlowstickColors[i]);

        instancedPlasticShader->use();
        instancedPlasticShader->setBool(name + ".isActive", true);
        instancedPlasticShader->setVec3(name + ".position", visibleGlowsticks[i]);
        instancedPlasticShader->setVec3(name + ".color", visibleGlowstickColors[i]);

        shovelShader->use();
        shovelShader->setBool(name + ".isActive", true);
        shovelShader->setVec3(name + ".position", visibleGlowsticks[i]);
        shovelShader->setVec3(name + ".color", visibleGlowstickColors[i]);
    }

    if (glowstickCount > visibleGlowsticks.size()) {
        for (int i = visibleGlowsticks.size(); i < glowstickCount; i++) {
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
    }

}

void LightsManager::TurnOffGlowsticks() {
    for(int i = 0; i < maxGlowsticks; i++) {
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