#include "InstanceRenderer.h"
#include "Core/Node.h"

InstanceRenderer::InstanceRenderer(Model* model, int maxSize, Shader* shader, bool isDynamic)
    : _model(model), _maxSize(maxSize), _shader(shader), _isDynamic(isDynamic), _castShadows(false) {
    CreatePositionBuffer(maxSize);
    SetupInstanceModel();
    _shadowShader = RESOURCEMANAGER.GetShaderByName("shadowInstanceShader");
}

InstanceRenderer::InstanceRenderer(): _castShadows(false){
    _type = ComponentType::INSTANCERENDERER;
    _shadowShader = RESOURCEMANAGER.GetShaderByName("shadowInstanceShader");
}

nlohmann::json InstanceRenderer::Serialize() {
    nlohmann::json data = Component::Serialize();

    data["model"] = _model->_name;
    data["maxSize"] = _maxSize;
    data["shader"] = _shader->_name;
    data["isDynamic"] = _isDynamic;
    data["castShadows"] = _castShadows;

    return data;
}

void InstanceRenderer::Deserialize(const nlohmann::json& jsonData) {

    if (jsonData.contains("model")) {
        _model = RESOURCEMANAGER.GetModelByName(jsonData["model"].get<std::string>());
    }

    if (jsonData.contains("maxSize")) {
        _maxSize = jsonData["maxSize"].get<int>();
    }

    if (jsonData.contains("shader")) {
        _shader = RESOURCEMANAGER.GetShaderByName(jsonData["shader"].get<std::string>());
    }

    if (jsonData.contains("isDynamic")) {
        _isDynamic = jsonData["isDynamic"].get<bool>();
    }

    if (jsonData.contains("castShadows")) {
        _castShadows = jsonData["castShadows"].get<bool>();
    }

    CreatePositionBuffer(_maxSize);
    SetupInstanceModel();

    Component::Deserialize(jsonData);
}

void InstanceRenderer::Initiate() {
    Component::Initiate();
}

void InstanceRenderer::Render(glm::mat4 parentWorld) {
    _shader->use();
    _model->InstanceDraw(*_shader, _instancePositionSize);
}

void InstanceRenderer::RenderShadows(glm::mat4 parentWorld) {
    if (!_castShadows) return;

    _shadowShader->use();
    _model->InstanceDraw(*_shadowShader, _instancePositionSize);
}

void InstanceRenderer::RefreshPositionBuffer(const std::vector<glm::vec3>& instancePositions) {
    _instancePositionSize = instancePositions.size();
    glBindBuffer(GL_ARRAY_BUFFER, _instanceBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _instancePositionSize * sizeof(glm::vec3), instancePositions.data());
}

void InstanceRenderer::SetupInstanceModel() {
    for (unsigned int i = 0; i < _model->meshes.size(); i++) {
        unsigned int VAO = _model->meshes[i].VAO;
        glBindVertexArray(VAO);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
        glVertexAttribDivisor(2, 1);

        glBindVertexArray(0);
    }
}

void InstanceRenderer::CreatePositionBuffer(int maxSize) {
    glGenBuffers(1, &_instanceBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _instanceBuffer);
    if (_isDynamic)
        glBufferData(GL_ARRAY_BUFFER, maxSize * sizeof(glm::vec3), nullptr, GL_DYNAMIC_DRAW);
    else
        glBufferData(GL_ARRAY_BUFFER, maxSize * sizeof(glm::vec3), nullptr, GL_STATIC_DRAW);
}
