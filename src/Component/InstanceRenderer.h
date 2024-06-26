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

    void SetShader(std::shared_ptr<Shader> newShader) {
        _shader = newShader;
    }

    void RefreshPositionBuffer(const std::vector<glm::vec3>& instancePositions);

private:
    std::shared_ptr<Model> _model;
    std::shared_ptr<Shader> _shader;
    std::shared_ptr<Shader> _shadowShader;
    unsigned int _instancePositionSize;
    unsigned int _maxSize;
    unsigned int _instanceBuffer;
    bool _isDynamic;
    bool _castShadows;

    void SetupInstanceModel();
    void CreatePositionBuffer(int maxSize);
};
