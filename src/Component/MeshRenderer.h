#pragma once
#include "Core/Component.h"
#include <glm/glm.hpp>
#include <model.h>

class MeshRenderer : public Component {
public:
    MeshRenderer(Model* model, Shader* shader);
    MeshRenderer(Shader* shader);
    Model *getModel() const;
    void setModel(Model *model);
    Shader *getShader() const;
    void setShader(Shader *shader);
    void Render(glm::mat4 parentWorld) override;
    void addToInspector(ImguiMain *imguiMain) override;

private:
    Model* _model;
    Shader* _shader;

    void RenderModel(Model* model, glm::mat4 ctm);

};
