#include "InstanceRenderer.h"

InstanceRenderer::InstanceRenderer(Model* model, int maxSize, Shader* shader, bool isDynamic)
    : _model(model),_maxSize(maxSize) , _shader(shader), _isDynamic(isDynamic) {
    CreateMatrixBuffer(maxSize);
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

void InstanceRenderer::Deserialize(const nlohmann::json &jsonData) {

    if (jsonData.contains("model")) {
        _model = RESOURCEMANAGER.GetModelByName(jsonData["model"].get<string>());
    }

    if (jsonData.contains("maxSize")) {
        _maxSize = jsonData["maxSize"].get<int>();
    }

    if (jsonData.contains("shader")) {
        _shader = RESOURCEMANAGER.GetShaderByName(jsonData["shader"].get<string>());
    }

    if (jsonData.contains("isDynamic")) {
        _isDynamic = jsonData["isDynamic"].get<bool>();
    }

    CreateMatrixBuffer(_maxSize);
    SetupInstanceModel();

    Component::Deserialize(jsonData);
}

void InstanceRenderer::Initiate() {
    Component::Initiate();
}

void InstanceRenderer::Render(glm::mat4 parentWorld) {
    _shader->use();
    _model->InstanceDraw(*_shader, _instanceMatrixSize);
}

void InstanceRenderer::RenderShadows(glm::mat4 parentWorld) {
    shared_ptr<Shader> currentShader = _shader;
    SetShader(RESOURCEMANAGER.GetShaderByName("shadowInstanceShader"));
    _shader->use();
    _model->InstanceDraw(*_shader, _instanceMatrixSize);
    SetShader(currentShader);
}

void InstanceRenderer::RefreshMatrixBuffer(const std::vector<glm::mat4>& instanceMatrix) {
    _instanceMatrixSize = instanceMatrix.size();
    glBindBuffer(GL_ARRAY_BUFFER, _instanceBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _instanceMatrixSize * sizeof(glm::mat4), instanceMatrix.data());
}

void InstanceRenderer::SetupInstanceModel() {
    for (unsigned int i = 0; i < _model->meshes.size(); i++)
    {
        unsigned int VAO = _model->meshes[i].VAO;
        glBindVertexArray(VAO);
        // set attribute pointers for matrix (4 times vec4)
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)0);
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(sizeof(glm::vec4)));
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(2 * sizeof(glm::vec4)));
        glEnableVertexAttribArray(6);
        glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(glm::mat4), (void*)(3 * sizeof(glm::vec4)));

        glVertexAttribDivisor(3, 1);
        glVertexAttribDivisor(4, 1);
        glVertexAttribDivisor(5, 1);
        glVertexAttribDivisor(6, 1);

        glBindVertexArray(0);
    }
}

void InstanceRenderer::CreateMatrixBuffer(int maxSize) {
    glGenBuffers(1, &_instanceBuffer);
    glBindBuffer(GL_ARRAY_BUFFER, _instanceBuffer);
    if(_isDynamic)
        glBufferData(GL_ARRAY_BUFFER, maxSize * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
    else
        glBufferData(GL_ARRAY_BUFFER, maxSize * sizeof(glm::mat4), nullptr, GL_STATIC_DRAW);
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
        if (ImGui::InputInt("Max Size", (int*)&_maxSize)) {};

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

