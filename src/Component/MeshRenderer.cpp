#include "MeshRenderer.h"
#include "Managers/TransparentRenderer.h"

MeshRenderer::MeshRenderer(Model* model, Shader* shader) : _model(model), _shader(shader) {}

MeshRenderer::MeshRenderer() {
    _type = ComponentType::MESHRENDERER;
}

void MeshRenderer::Render(glm::mat4 parentWorld) {
    glm::mat4 world = _ownerTransform->Combine(parentWorld);

    if (FrustumCulling::IsInFrustum(_cameraRef->getViewProjectionMatrix(), world, _cameraRef->getFrustumPlanes(), _model)) {
        isInFrustum = true;
        if (!_disableModel) {
            if (!_transparent)
                RenderModel(world);
            else
                TRANSPARENT_RENDERER.AddModel(shared_from_this(), world);
        }
    }
    else {
        isInFrustum = false;
    }
}

void MeshRenderer::RenderShadows(glm::mat4 parentWorld) {
    if (_disableShadows) return;

    std::shared_ptr<Shader> currentShader = _shader;
    SetShader(RESOURCEMANAGER.GetShaderByName("shadowShader"));
    glm::mat4 world = _ownerTransform->Combine(parentWorld);
    RenderModel(world);
    SetShader(currentShader);
}

void MeshRenderer::RenderModel(glm::mat4 ctm) {
    if (_shouldRenderOutline) {
        glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);
        glStencilFunc(GL_ALWAYS, 1, 0xFF); // all fragments should pass the stencil test
        glStencilMask(0xFF); // enable writing to the stencil buffer
    }

    _shader->use();
    _shader->setMat4("model", ctm);
    _model->Draw(*_shader);

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

void MeshRenderer::Initiate() {
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(2);
    Component::Initiate();
}

void MeshRenderer::Update() {
    _ownerTransform->SetPosition(_ownerTransform->GetPosition());
    _ownerTransform->SetScale(_ownerTransform->GetScale());
    _ownerTransform->SetRotation(_ownerTransform->GetRotation());
    Component::Update();
}
