#pragma once

#include "Core/Component.h"
#include <model.h>
#include <vector>
#include "../Managers/ResourceManager.h"

class InstanceRenderer : public Component {
public:
    InstanceRenderer(Model* model, int maxSize, Shader* shader);

    InstanceRenderer();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json& jsonData) override;

    void Initiate() override;

    void Render(glm::mat4 parentWorld) override;

    void RenderShadows(glm::mat4 parentWorld) override;

    void SetShader(shared_ptr<Shader> newShader) {
        _shader = newShader;
    }

    void RefreshMatrixBuffer();

    // Getters
    const std::vector<glm::mat4>& GetInstanceMatrix() const { return _instanceMatrix; }

    void addToInspector(ImguiMain *imguiMain) override;

    // Setters
    void SetInstanceMatrix(const std::vector<glm::mat4>& instanceMatrix) { _instanceMatrix = instanceMatrix; RefreshMatrixBuffer(); }

//private:
    shared_ptr<Model> _model;
    std::vector<glm::mat4> _instanceMatrix;
    shared_ptr<Shader> _shader;
    unsigned int _maxSize;
    unsigned int _instanceBuffer;

    void SetupInstanceModel();
    void CreateMatrixBuffer(int maxSize);
};
