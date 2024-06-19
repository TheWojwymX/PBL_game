//
// Created by Jacek on 31.05.2024.
//

#include "ShovelRenderer.h"


ShovelRenderer::ShovelRenderer(Model* model, Shader* shader) : _model(model), _shader(shader) {}

ShovelRenderer::ShovelRenderer(){
    _type = ComponentType::SHOVELRENDERER;
}

void ShovelRenderer::RenderShovel(glm::mat4 parentWorld) {

    glm::mat4 world = _ownerTransform->Combine(parentWorld);
    _shader->use();
    _shader->setMat4("model", world);
    _model->Draw(*_shader);
}

nlohmann::json ShovelRenderer::Serialize() {
    nlohmann::json data = Component::Serialize();

    data["model"] = _model->_name;
    data["shader"] = _shader->_name;

    return data;
}

void ShovelRenderer::Deserialize(const nlohmann::json &jsonData) {

    if (jsonData.contains("model")) {
        _model = RESOURCEMANAGER.GetModelByName(jsonData["model"].get<string>());
    }

    if (jsonData.contains("shader")) {
        _shader = RESOURCEMANAGER.GetShaderByName(jsonData["shader"].get<string>());
    }

    Component::Deserialize(jsonData);
}

void ShovelRenderer::Initiate() {
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(2);
    framesRendered = 0;
    Component::Initiate();
}

void ShovelRenderer::Update() {

    _ownerTransform->SetPosition(_ownerTransform->GetPosition());
    _ownerTransform->SetScale(_ownerTransform->GetScale());
    _ownerTransform->SetRotation(_ownerTransform->GetRotation());

    Component::Update();
}