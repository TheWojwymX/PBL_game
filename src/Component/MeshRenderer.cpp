#include "MeshRenderer.h"

MeshRenderer::MeshRenderer(Model* model, Shader* shader) : _model(model), _shader(shader) {}

void MeshRenderer::Render(glm::mat4 parentWorld) {
    glm::mat4 world = _ownerTransform->Combine(parentWorld);
    RenderModel(_model, world);
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
