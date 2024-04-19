#pragma once
#include "Core/Component.h"
#include <glm/glm.hpp>
#include <model.h>
#include "Managers/ResourceManager.h"

class MeshRenderer : public Component {
public:
    MeshRenderer();

    MeshRenderer(Model* model, Shader* shader);

    void Render(glm::mat4 parentWorld) override;

    void addToInspector(ImguiMain *imguiMain) override;

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void initiate() override;

    bool _shouldRenderOutline = false;

private:
    shared_ptr<Model> _model; // Raw pointer to the model
    shared_ptr<Shader> _shader; // Raw pointer to the shader
    shared_ptr<Shader> _outlineShader; // Raw pointer to the outline shader

    void RenderModel(shared_ptr<Model> model, glm::mat4 ctm);
};
