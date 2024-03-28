#pragma once

#include <vector>
#include <memory>
#include "Core/Component.h"
#include "Component/InstanceRenderer.h"
#include "Component/Camera.h"
#include "BlockData.h"
#include "imgui.h"


// Define a struct to hold collision information for each corner
struct CollisionInfo {
    bool isColliding;
    glm::vec3 separationVector;
};


class BlockManager : public Component, public std::enable_shared_from_this<BlockManager> {
public:
    BlockManager(int width, int height, int depth);

    BlockManager();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json& jsonData) override;

    void Init() override;

    bool RayIntersectsBlock(float rayLength);
    glm::vec3 CheckEntityCollision(const glm::vec3& entityPos, float entityWidth, float entityHeight);

    void SetInstanceRenderer(std::shared_ptr<InstanceRenderer> renderer) { _sandRendererRef = renderer; }
    void SetCamera(std::shared_ptr<Camera> camera) { _cameraRef = camera; }

    void addToInspector(ImguiMain *imguiMain) override;

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
    std::vector<CollisionInfo> CalculateCollisionInfo(const glm::vec3& entityPos, float halfWidth, float entityHeight);
   
    int GetIndex(glm::ivec3 point);
    int GetIndex(int x, int y, int z);
    bool CheckAdjacency(int x, int y, int z);
    bool InBounds(glm::ivec3 position);
};
