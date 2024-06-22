#pragma once

#include "Core/Component.h"
#include "Core/Node.h"
#include "Component/BlockManager.h"

class TopLayerSnap : public Component {
public:
    TopLayerSnap();

    void Deserialize(const nlohmann::json& jsonData) override;
    nlohmann::json Serialize() override;

    void Init() override;
    void Update() override;

    void Reset();

private:
    std::shared_ptr<BlockManager> _blockManagerRef;
    float _yOffset;
};