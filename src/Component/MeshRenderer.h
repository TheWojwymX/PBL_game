#pragma once
#include "Core/Component.h"
#include <glm/glm.hpp>
#include <model.h>

class MeshRenderer : public Component {
public:
    MeshRenderer(Model* model, Shader* shader);
    void Render(glm::mat4 parentWorld) override;

private:
    Model* _model; // Raw pointer to the model
    Shader* _shader; // Raw pointer to the shader

    void RenderModel(Model* model, glm::mat4 ctm);
};
