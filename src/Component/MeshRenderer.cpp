#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Model* model, Shader* shader) : _model(model), _shader(shader) {}

MeshRenderer::MeshRenderer(Shader *shader): _shader(shader) {}

void MeshRenderer::Render(glm::mat4 parentWorld) {
    glm::mat4 world = _ownerTransform->Combine(parentWorld);
    RenderModel(_model, parentWorld);
}

void MeshRenderer::RenderModel(Model* model, glm::mat4 ctm) {
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

Model *MeshRenderer::getModel() const {
    return _model;
}

void MeshRenderer::setModel(Model *model) {
    _model = model;
}

Shader *MeshRenderer::getShader() const {
    return _shader;
}

void MeshRenderer::setShader(Shader *shader) {
    _shader = shader;
}
