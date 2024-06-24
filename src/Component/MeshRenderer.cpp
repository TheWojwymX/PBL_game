#include "MeshRenderer.h"
#include "Managers/TransparentRenderer.h"
#include "Core/Node.h"

MeshRenderer::MeshRenderer(Model* model, Shader* shader) : _model(model), _shader(shader) {
    _shadowShader = RESOURCEMANAGER.GetShaderByName("shadowShader");
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(2);
}

MeshRenderer::MeshRenderer() {
    _type = ComponentType::MESHRENDERER;
    _shadowShader = RESOURCEMANAGER.GetShaderByName("shadowShader");
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(2);
}

void MeshRenderer::Render(glm::mat4 parentWorld) {
    if (_disableModel) return;
    glm::mat4 world = _ownerTransform->Combine(parentWorld);

    if (FrustumCulling::IsInFrustum(_cameraRef->getViewProjectionMatrix(), world, _cameraRef->getFrustumPlanes(), _model)) {
        if (!_transparent)
            RenderModel(world);
        else
            TRANSPARENT_RENDERER.AddModel(shared_from_this(), world);
    }
}

void MeshRenderer::RenderShadows(glm::mat4 parentWorld) {
    if (_disableShadows) return;
    glm::mat4 world = _ownerTransform->Combine(parentWorld);

    RenderModel(world,true);
}

void MeshRenderer::RenderModel(glm::mat4 ctm, bool isShadow) {
    if (_shouldRenderOutline) {
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
        glStencilMask(0xFF); // enable writing to the stencil buffer
    }

    if (isShadow)
    {
        _shadowShader->use();
        _shadowShader->setMat4("model", ctm);
        _model->Draw(*_shadowShader);
    }
    else {
        _shader->use();
        _shader->setMat4("model", ctm);
        _model->Draw(*_shader);
    }


    if (_shouldRenderOutline) {
        glStencilFunc(GL_NOTEQUAL, 1, 0xFF);
        glStencilMask(0x00); // disable writing to the stencil buffer

        _outlineShader->use();
        _outlineShader->setMat4("model", ctm);
        _model->Draw(*_outlineShader);

        glStencilMask(0xFF);
        glStencilFunc(GL_ALWAYS, 0, 0xFF);
    }
}

void MeshRenderer::addToInspector(ImguiMain* imguiMain) {
    if (ImGui::TreeNode("Mesh Renderer")) {
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

void MeshRenderer::Deserialize(const nlohmann::json& jsonData) {
    if (jsonData.contains("model")) {
        _model = RESOURCEMANAGER.GetModelByName(jsonData["model"].get<std::string>());
    }

    if (jsonData.contains("shader")) {
        _shader = RESOURCEMANAGER.GetShaderByName(jsonData["shader"].get<std::string>());
    }

    if (jsonData.contains("outlineShader")) {
        _outlineShader = RESOURCEMANAGER.GetShaderByName(jsonData["outlineShader"].get<std::string>());
    }

    Component::Deserialize(jsonData);
}
