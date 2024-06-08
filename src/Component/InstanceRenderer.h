#pragma once

#include "Core/Component.h"
#include <model.h>
#include <vector>
#include "../Managers/ResourceManager.h"

class InstanceRenderer : public Component {
public:
    InstanceRenderer(Model* model, int maxSize, Shader* shader, bool isDynamic);

    InstanceRenderer();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json& jsonData) override;

    void Initiate() override;

    void Render(glm::mat4 parentWorld) override;

    void RenderShadows(glm::mat4 parentWorld) override;

    void SetShader(shared_ptr<Shader> newShader) {
        _shader = newShader;
    }

    void RefreshMatrixBuffer(const std::vector<glm::mat4>& instanceMatrix);

    void addToInspector(ImguiMain *imguiMain) override;

private:
    shared_ptr<Model> _model;
    shared_ptr<Shader> _shader;
    unsigned int _instanceMatrixSize;
    unsigned int _maxSize;
    unsigned int _instanceBuffer;
    bool _isDynamic;

    void SetupInstanceModel();
    void CreateMatrixBuffer(int maxSize);
};
