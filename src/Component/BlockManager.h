#pragma once

#include <vector>
#include <memory>
#include "Core/Component.h"
#include "Component/InstanceRenderer.h"
#include "Component/Camera.h"
#include "BlockData.h"

class BlockManager : public Component, public std::enable_shared_from_this<BlockManager> {
public:
    nlohmann::json Serialize() const;

    BlockManager(int width, int height, int depth);

    void Init() override;

    bool RayIntersectsBlock(float rayLength);
    bool CheckEntityCollision(const glm::vec3& entityPos, float entityWidth, float entityHeight);

    void SetInstanceRenderer(std::shared_ptr<InstanceRenderer> renderer) { _sandRendererRef = renderer; }
    void SetCamera(std::shared_ptr<Camera> camera) { _cameraRef = camera; }

private:
    int _width;
    int _depth;
    int _height;
    std::vector<BlockData> _blocksData;
    std::vector<BlockData*> _visibleBlocks;
    std::shared_ptr<InstanceRenderer> _sandRendererRef;
    std::shared_ptr<Camera> _cameraRef;

    void GenerateMap();
    void UpdateInstanceRenderer();
    void RefreshVisibleBlocks();
    void UpdateBlocksVisibility();
    void UpdateBlockVisibility(BlockData& blockData);
    void UpdateNeighbourVisibility(BlockData& blockData);
    void SetVisibility(BlockData& blockData, bool state);
   
    int GetIndex(glm::ivec3 point);
    int GetIndex(int x, int y, int z);
    bool CheckAdjacency(int x, int y, int z);
};
