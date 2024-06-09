#include "InstanceRenderer.h"

InstanceRenderer::InstanceRenderer(Model* model, int maxSize, Shader* shader, bool isDynamic)
    : _model(model), _maxSize(maxSize), _shader(shader), _isDynamic(isDynamic) {
    CreatePositionBuffer(maxSize);
    SetupInstanceModel();
}

InstanceRenderer::InstanceRenderer() {
    _type = ComponentType::INSTANCERENDERER;
}

nlohmann::json InstanceRenderer::Serialize() {
    nlohmann::json data = Component::Serialize();

    data["model"] = _model->_name;
    data["maxSize"] = _maxSize;
    data["shader"] = _shader->_name;
    data["isDynamic"] = _isDynamic;

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
    std::shared_ptr<Shader> currentShader = _shader;
    SetShader(RESOURCEMANAGER.GetShaderByName("shadowInstanceShader"));
    _shader->use();
    _model->InstanceDraw(*_shader, _instancePositionSize);
    SetShader(currentShader);
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

void InstanceRenderer::addToInspector(ImguiMain* imguiMain)
{
    if (ImGui::TreeNode("Instance Renderer"))
    {
        // Display and edit the model name
        char modelName[128];
        strcpy(modelName, _model ? _model->_name.c_str() : "");
        if (ImGui::InputText("Model Name", modelName, sizeof(modelName)))
        {
            _model = RESOURCEMANAGER.GetModelByName(modelName);
        }

        // Display and edit the max size
        int maxSize = static_cast<int>(_maxSize); // Use a temporary variable for ImGui
        if (ImGui::InputInt("Max Size", &maxSize)) {
            _maxSize = static_cast<unsigned int>(maxSize);
        }

        // Display and edit the shader name
        char shaderName[128];
        strcpy(shaderName, _shader ? _shader->_name.c_str() : "");
        if (ImGui::InputText("Shader Name", shaderName, sizeof(shaderName)))
        {
            _shader = RESOURCEMANAGER.GetShaderByName(shaderName);
        }

        // Display and edit the dynamic flag
        ImGui::Checkbox("Is Dynamic", &_isDynamic);

        ImGui::TreePop();
    }
}
