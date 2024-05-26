#pragma once

#include <vector>
#include <memory>
#include <random>
#include <unordered_set>
#include "Core/Component.h"
#include "Component/InstanceRenderer.h"
#include "Component/Camera.h"
#include "BlockData.h"
#include "imgui.h"
#include "Managers/ComponentsManager.h"


// Define a struct to hold collision information for each corner
struct CollisionInfo {
    bool isColliding = false;;
    glm::vec3 separationVector = glm::vec3(std::numeric_limits<float>::max());
    bool isGrounded = false;
};


class BlockManager : public Component, public std::enable_shared_from_this<BlockManager> {
public:
    BlockManager(int width, int height, int depth);

    BlockManager();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json& jsonData) override;

    void Initiate() override;

    void Init() override;

    bool RayIntersectsBlock(float rayLength, int radius, float digPower);
    std::pair<glm::vec3,glm::vec3> CheckEntityCollision(glm::vec3 entityPos, glm::vec3 movementVector, float entityWidth, float entityHeight);

    void SetInstanceRenderer(std::shared_ptr<InstanceRenderer> renderer) { _sandRendererRef = renderer; }
    void SetCamera(std::shared_ptr<Camera> camera) { _cameraRef = camera; }

    void addToInspector(ImguiMain *imguiMain) override;

private:
    int _width;
    int _depth;
    int _height;
    int _chunkSize;
    int _renderDistance;
    glm::ivec3 _playerChunk;
    std::vector<BlockData> _blocksData;
    std::vector<std::vector<BlockData*>> _visibleBlocks;
    std::vector<int> _renderedChunks;
    std::shared_ptr<InstanceRenderer> _sandRendererRef;
    int _sandRendererRefID;
    std::shared_ptr<InstanceRenderer> _topLayerRendererRef;
    int _topLayerRendererRefID;
    std::shared_ptr<Camera> _cameraRef;
    std::vector<std::vector<glm::ivec3>> _sphereVectors;
    int _cameraRefID;
    int _entranceMask[96] = { 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0, 0, 1, 1, 0, 0, 1, 1, 1, 1, 1 };

    void UpdateInstanceRenderer();
    void RefreshVisibleBlocks();
    void RefreshVisibleBlocks(int chunkIndex);
    void UpdateBlocksVisibility();
    void UpdateRenderedChunks();
    void DamageBlocks(glm::ivec3 hitPos, int radius, float digPower);
    void HideEdges();
    void UpdateBlockVisibility(BlockData& blockData);
    void UpdateNeighbourVisibility(BlockData& blockData);
    void SetVisibility(BlockData& blockData, bool state);
    void GenerateSphereVectors(int radius);
    std::vector<CollisionInfo> CalculateCollisionInfo(glm::vec3 entityPos, glm::vec3 movementVector, float halfWidth, float entityHeight);
    void GenerateMap(float initialFillRatio, int numIterations);
    void GenerateTopLayer(int sizeX, int sizeZ);
    void InitializeMap(float initialFillRatio);
    void IterateCaveGeneration();
    void CheckEntityChunk(glm::vec3 entityPos);
    void ChangeType(BlockData& blockData, BlockType type);
    void ApplyMask(glm::ivec3 startPos, int* maskArray, glm::ivec3 maskDimensions);


    int GetIndex(glm::ivec3 point);
    int GetIndex(int x, int y, int z);
    int GetChunkIndex(glm::ivec3 point);
    int GetChunkIndex(int x, int y, int z);
    bool CheckAdjacency(int x, int y, int z);
    bool InBounds(glm::ivec3 position);
    bool InBounds(int x, int y, int z);
    bool ChunkInBounds(glm::ivec3 position);
    bool IsEdgeBlock(int x, int y, int z);
};
