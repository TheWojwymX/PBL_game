#include "LightsManager.h"


LightsManager &LightsManager::getInstance() {
    static LightsManager instance;
    return instance;
}

void LightsManager::UpdateLights() {
    flashlightCurrentPosition = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->LerpPosition(flashlightCurrentPosition);
    flashlightCurrentDirection = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->LerpDirection(flashlightCurrentDirection);

    UpdateShaders();
    UpdateGlowsticks();
}

void LightsManager::InitLights() {
    instancedSandShader = RESOURCEMANAGER.GetShaderByName("instancedSandShader");
    instancedMetalShader = RESOURCEMANAGER.GetShaderByName("instancedMetalShader");
    modelShader = RESOURCEMANAGER.GetShaderByName("modelShader");
    shovelShader = RESOURCEMANAGER.GetShaderByName("shovelShader");

    //Flashlight Properties
    isSpotActive = true;
    flashlightCurrentPosition = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition();
    flashlightCurrentDirection = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetFrontVector();
}

void LightsManager::UpdateShaders(){
    modelShader->use();
    modelShader->setBool("spotLights[0].isActive", isSpotActive);
    modelShader->setVec3("spotLights[0].position", flashlightCurrentPosition);
    modelShader->setVec3("spotLights[0].direction", flashlightCurrentDirection);
    modelShader->setFloat("spotLights[0].constant", flashlightConstant);
    modelShader->setFloat("spotLights[0].linear", flashlightLinear);
    modelShader->setFloat("spotLights[0].quadratic", flashlightQuadratic);
    modelShader->setVec3("spotLights[0].color", flashlightColor);
    modelShader->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(flashlightCutOff)));
    modelShader->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(flashlightOuterCutOff)));

    instancedSandShader->use();
    instancedSandShader->setBool("spotLights[0].isActive", isSpotActive);
    instancedSandShader->setVec3("spotLights[0].position", flashlightCurrentPosition);
    instancedSandShader->setVec3("spotLights[0].direction", flashlightCurrentDirection);
    instancedSandShader->setFloat("spotLights[0].constant", flashlightConstant);
    instancedSandShader->setFloat("spotLights[0].linear", flashlightLinear);
    instancedSandShader->setFloat("spotLights[0].quadratic", flashlightQuadratic);
    instancedSandShader->setVec3("spotLights[0].color", flashlightColor);
    instancedSandShader->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(flashlightCutOff)));
    instancedSandShader->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(flashlightOuterCutOff)));

    for (int i = 0; i < maxGlowsticks; i++) {
        string name = "pointLights[" + to_string(i) + "]";
        if(isSpotActive) {
            instancedSandShader->setFloat(name + ".constant", glowstickConstant);
            instancedSandShader->setFloat(name + ".linear", glowstickLinear);
            instancedSandShader->setFloat(name + ".quadratic", glowstickQuadratic);
        }
        else{
            instancedSandShader->setFloat(name + ".constant", glowstickConstantNoFlash);
            instancedSandShader->setFloat(name + ".linear", glowstickLinearNoFlash);
            instancedSandShader->setFloat(name + ".quadratic", glowstickQuadraticNoFlash);
        }
    }

    instancedMetalShader->use();
    instancedMetalShader->setBool("spotLights[0].isActive", isSpotActive);
    instancedMetalShader->setVec3("spotLights[0].position", flashlightCurrentPosition);
    instancedMetalShader->setVec3("spotLights[0].direction", flashlightCurrentDirection);
    instancedMetalShader->setFloat("spotLights[0].constant", flashlightConstant);
    instancedMetalShader->setFloat("spotLights[0].linear", flashlightLinear);
    instancedMetalShader->setFloat("spotLights[0].quadratic", flashlightQuadratic);
    instancedMetalShader->setVec3("spotLights[0].color", flashlightColor);
    instancedMetalShader->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(flashlightCutOff)));
    instancedMetalShader->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(flashlightOuterCutOff)));

    shovelShader->use();
    shovelShader->setBool("spotLights[0].isActive", isSpotActive);
    shovelShader->setVec3("spotLights[0].position", flashlightCurrentPosition);
    shovelShader->setVec3("spotLights[0].direction", flashlightCurrentDirection);
    shovelShader->setFloat("spotLights[0].constant", flashlightConstant);
    shovelShader->setFloat("spotLights[0].linear", flashlightLinear);
    shovelShader->setFloat("spotLights[0].quadratic", flashlightQuadratic);
    shovelShader->setVec3("spotLights[0].color", flashlightColor);
    shovelShader->setFloat("spotLights[0].cutOff", glm::cos(glm::radians(flashlightCutOff)));
    shovelShader->setFloat("spotLights[0].outerCutOff", glm::cos(glm::radians(flashlightOuterCutOff)));
}

void LightsManager::AddGlowstick() {
    glowstickCount++;
    std::string glowstickNodeName = "Glowstick" + to_string(glowstickCount);
    shared_ptr<Node> glowstickNode = NODESMANAGER.createNode(NODESMANAGER.getNodeByName("root"), glowstickNodeName);
    glowstickNode->GetTransform()->SetPosition(ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition());
    glowstickNode->GetTransform()->SetScale(glm::vec3(1.4, 1.4, 1.4));

    auto glowstickMeshRenderer = COMPONENTSMANAGER.CreateComponent<MeshRenderer>();
    glowstickMeshRenderer->_model = RESOURCEMANAGER.GetModelByName("glowstickModel");
    glowstickMeshRenderer->_shader = RESOURCEMANAGER.GetShaderByName("modelShader");
    glowstickMeshRenderer->_outlineShader = RESOURCEMANAGER.GetShaderByName("outlineShader");
    glowstickMeshRenderer->Initiate();
    glowstickNode->AddComponent(glowstickMeshRenderer);

    auto glowstickMovement = COMPONENTSMANAGER.CreateComponent<GlowstickMovement>();
    glowstickNode->AddComponent(glowstickMovement);
    glowstickMovement->Init();

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(-2.0f, 2.0f);

    do {
        glowstickColor = glm::vec3(dis(gen), dis(gen), dis(gen));
    } while (glowstickColor.x <= 0.5f && glowstickColor.y <= 0.5f && glowstickColor.z <= 0.5f);
    glowstickColors.push_back(glowstickColor);

    modelShader->use();
    modelShader->setVec3("glowstickColor", glowstickColor);
}

void LightsManager::UpdateGlowsticks() {
    glm::vec3 camPosition = ComponentsManager::getInstance().GetComponentByID<Camera>(2)->GetPosition();
    std::vector<glm::vec3> visibleGlowsticks;
    std::vector<glm::vec3> visibleGlowstickColors;

    for (int i = 0; i < glowstickCount; i++) {
        auto thisGlowstick = NODESMANAGER.getNodeByName("Glowstick" + to_string(i + 1));
        glm::vec3 glowstickPosition = thisGlowstick->GetTransform()->GetPosition();

        if (glm::distance(glowstickPosition, camPosition) < 10.0f || thisGlowstick->GetComponent<MeshRenderer>()->isInFrustum) {
            visibleGlowsticks.push_back(glowstickPosition);
            visibleGlowstickColors.push_back(glowstickColors[i]);
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
    }

    if (glowstickCount > visibleGlowsticks.size()) {
        for (int i = visibleGlowsticks.size(); i < glowstickCount; i++) {
            string name = "pointLights[" + to_string(i) + "]";
            instancedSandShader->setBool(name + ".isActive", false);
        }
    }

}