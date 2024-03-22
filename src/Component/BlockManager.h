#pragma once

#include <vector>
#include <tuple>
#include <memory>
#include "Core/Component.h"
#include "Component/InstanceRenderer.h"
#include "Component/Camera.h"
#include "BlockData.h"

class BlockManager : public Component, public std::enable_shared_from_this<BlockManager> {
public:
    BlockManager(int width, int height, int depth);

    void Init() override;

    bool RayIntersectsBlock(float rayLength);

    void SetInstanceRenderer(std::shared_ptr<InstanceRenderer> renderer) { _sandRendererRef = renderer; }
    void SetCamera(std::shared_ptr<Camera> camera) { _cameraRef = camera; }

private:
    int _width;
    int _depth;
    int _height;
    std::vector<std::tuple<BlockData, glm::mat4>> _blocksData;
    std::vector<BlockData*> _visibleBlocks;
    std::shared_ptr<InstanceRenderer> _sandRendererRef;
    std::shared_ptr<Camera> _cameraRef;

    void GenerateMap();
    void UpdateInstanceRenderer();
    void UpdateBlocksVisibility();
    void UpdateBlockVisibility(BlockData& blockData);
    int GetIndex(glm::ivec3 point);
    int GetIndex(int x, int y, int z);
    bool CheckAdjacency(int x, int y, int z);
};
