#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Model* model, Shader* shader) : _model(model), _shader(shader) {}

MeshRenderer::MeshRenderer(){
    _type = ComponentType::MESHRENDERER;
}

void MeshRenderer::Render(glm::mat4 parentWorld) {
    glm::mat4 world = _ownerTransform->Combine(parentWorld);
    RenderModel(_model, world);
}

void MeshRenderer::RenderModel(shared_ptr<Model> model, glm::mat4 ctm) {
    _shader->use();
    _shader->setMat4("model", ctm);
    model->Draw(*_shader);
}

void MeshRenderer::addToInspector(ImguiMain *imguiMain)
{
    if (ImGui::TreeNode("Mesh Renderer"))
    {
        // Block Gui related things go there
        ImGui::Text("Test1:");
        ImGui::Text("Test2:");

        ImGui::TreePop();

    }
}

nlohmann::json MeshRenderer::Serialize() {
    nlohmann::json data = Component::Serialize();

    data["model"] = _model->_name;
    data["shader"] = _shader->_name;

    return data;
}

void MeshRenderer::Deserialize(const nlohmann::json &jsonData) {

    if (jsonData.contains("model")) {
        _model = RESOURCEMANAGER.GetModelByName(jsonData["model"].get<string>());
    }

    if (jsonData.contains("shader")) {
        _shader = RESOURCEMANAGER.GetShaderByName(jsonData["shader"].get<string>());
    }

    Component::Deserialize(jsonData);
}

void MeshRenderer::initiate() {
    Component::initiate();
}
