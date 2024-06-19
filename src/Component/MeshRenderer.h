#pragma once

#include "Core/Component.h"
#include "Managers/ResourceManager.h"
#include "Component/Camera.h"
#include "Component/FrustumCulling.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/string_cast.hpp>

class TransparentRenderer;  

class MeshRenderer : public Component, public std::enable_shared_from_this<MeshRenderer> {
public:
    MeshRenderer();
    MeshRenderer(Model* model, Shader* shader);

    void Render(glm::mat4 parentWorld) override;
    void RenderShadows(glm::mat4 parentWorld) override;
    void addToInspector(ImguiMain* imguiMain) override;
    nlohmann::json Serialize() override;
    void Deserialize(const nlohmann::json& jsonData) override;
    void Initiate() override;
    void Update() override;

    void SetShader(std::shared_ptr<Shader> newShader) {
        _shader = newShader;
    }

    bool _shouldRenderOutline = false;

    std::shared_ptr<Model> _model;
    std::shared_ptr<Shader> _shader;
    std::shared_ptr<Shader> _outlineShader;
    std::shared_ptr<Camera> _cameraRef;

    bool isInFrustum = false;

    void RenderModel(glm::mat4 ctm);

    bool _disableModel = false;
    bool _disableShadows = false;
    bool _transparent = false;
};
