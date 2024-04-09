#include "InstanceRenderer.h"

InstanceRenderer::InstanceRenderer(Model* model, int maxSize, Shader* shader)
    : _model(model),_maxSize(maxSize) , _shader(shader) {
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

    CreateMatrixBuffer(_maxSize);
    SetupInstanceModel();

    Component::Deserialize(jsonData);
}

void InstanceRenderer::initiate() {
    Component::initiate();
}

void InstanceRenderer::Render(glm::mat4 parentWorld) {
    _shader->use();
    _model->InstanceDraw(*_shader, _instanceMatrix.size());
}

void InstanceRenderer::RefreshMatrixBuffer() {
    glBindBuffer(GL_ARRAY_BUFFER, _instanceBuffer);
    glBufferSubData(GL_ARRAY_BUFFER, 0, _instanceMatrix.size() * sizeof(glm::mat4), _instanceMatrix.data());
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
    glBufferData(GL_ARRAY_BUFFER, maxSize * sizeof(glm::mat4), nullptr, GL_DYNAMIC_DRAW);
}

void InstanceRenderer::addToInspector(ImguiMain *imguiMain)
{

}
