#pragma once

#include "Managers/ResourceManager.h"
#include "Managers/ComponentsManager.h"
#include "Component/Camera.h"
#include "Component/FrustumCulling.h"

class MeshRenderer : public Component {
public:
    MeshRenderer();

    MeshRenderer(Model* model, Shader* shader);

    void Render(glm::mat4 parentWorld) override;

    void RenderShadows(glm::mat4 parentWorld) override;

    void addToInspector(ImguiMain *imguiMain) override;

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void Initiate() override;

    void SetShader(shared_ptr<Shader> newShader) {
        _shader = newShader;
    }

    void Update() override;

    bool _shouldRenderOutline = false;

    shared_ptr<Model> _model; // Raw pointer to the model
    shared_ptr<Shader> _shader; // Raw pointer to the shader
    shared_ptr<Shader> _outlineShader; // Raw pointer to the outline shader
    std::shared_ptr<Camera> _cameraRef;

    bool isInFrustum = false;

    void RenderModel(shared_ptr<Model> model, glm::mat4 ctm);
};
