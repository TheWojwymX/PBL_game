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
    if(_shouldRenderOutline) {
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);

        glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
        glStencilMask(0xFF); // enable writing to the stencil buffer
    }

    _shader->use();
    _shader->setMat4("model", ctm);
    model->Draw(*_shader);

    if(_shouldRenderOutline) {
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00); // disable writing to the stencil buffer

        _outlineShader->use();
        _outlineShader->setMat4("model", ctm);
        model->Draw(*_outlineShader);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
    }
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
    data["outlineShader"] = _outlineShader->_name;

    return data;
}

void MeshRenderer::Deserialize(const nlohmann::json &jsonData) {

    if (jsonData.contains("model")) {
        _model = RESOURCEMANAGER.GetModelByName(jsonData["model"].get<string>());
    }

    if (jsonData.contains("shader")) {
        _shader = RESOURCEMANAGER.GetShaderByName(jsonData["shader"].get<string>());
    }

    if (jsonData.contains("shader")) {
        _outlineShader = RESOURCEMANAGER.GetShaderByName(jsonData["outlineShader"].get<string>());
    }

    Component::Deserialize(jsonData);
}

void MeshRenderer::initiate() {
    Component::initiate();
}
