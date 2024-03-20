#pragma once

#include <vector>
#include <tuple>
#include <memory>
#include "Core/Component.h"
#include "Component/InstanceRenderer.h"
#include "BlockData.h"

class BlockManager : public Component, public std::enable_shared_from_this<BlockManager> {
public:
    BlockManager(int width, int height, int depth);

    void Init() override;

    void SetInstanceRenderer(std::shared_ptr<InstanceRenderer> renderer);

private:
    int _width;
    int _depth;
    int _height;
    std::vector<std::tuple<BlockData, glm::mat4>> _blockData;
    std::shared_ptr<InstanceRenderer> _sandRendererRef;

    void GenerateMap();
    void UpdateInstanceRenderer();
};
