#pragma once

#include "Core/Component.h"
#include <model.h>
#include <vector>

class InstanceRenderer : public Component {
public:
    nlohmann::json Serialize() const;

    InstanceRenderer(Model* model, int maxSize, Shader* shader);

    void Render(glm::mat4 parentWorld) override;

    void RefreshMatrixBuffer();

    // Getters
    const std::vector<glm::mat4>& GetInstanceMatrix() const { return _instanceMatrix; }

    // Setters
    void SetInstanceMatrix(const std::vector<glm::mat4>& instanceMatrix) { _instanceMatrix = instanceMatrix; RefreshMatrixBuffer(); }

private:
    Model* _model;
    std::vector<glm::mat4> _instanceMatrix;
    Shader* _shader;
    unsigned int _maxSize;
    unsigned int _instanceBuffer;

    void SetupInstanceModel();
    void CreateMatrixBuffer(int maxSize);
};
