#pragma once

#include <vector>
#include <memory>
#include <random>
#include <glm/gtc/noise.hpp> 
#include <unordered_set>
#include <glm/gtc/matrix_transform.hpp>
#include "Core/Component.h"
#include "Component/InstanceRenderer.h"
#include "Component/Camera.h"
#include "BlockData.h"
#include "imgui.h"
#include "Managers/ComponentsManager.h"
#include "Managers/GameManager.h"


// Define a struct to hold collision information for each corner
struct CollisionInfo {
    bool isColliding = false;
    glm::vec3 separationVector = glm::vec3(std::numeric_limits<float>::max());
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
    std::pair<glm::vec3, glm::vec3> CheckEntityCollision(glm::vec3 entityPos, glm::vec3 movementVector, float entityWidth, float entityHeight);
    std::tuple<bool, BlockData*, glm::vec3> CheckSimpleEntityCollision(glm::vec3 entityPos);
    float GetCaveFloor(glm::vec3 entityPos, int maxDistance);
    float GetTopLayerFloor(glm::vec3 entityPos);

    void SetInstanceRenderer(std::shared_ptr<InstanceRenderer> renderer) { _sandRendererRef = renderer; }
    void SetCamera(std::shared_ptr<Camera> camera) { _cameraRef = camera; }

    void addToInspector(ImguiMain *imguiMain) override;

private:
    int _width;
    int _depth;
    int _height;
    int _chunkSize;
    int _renderDistance;
    std::vector<std::pair<float, int>> _layerStats;
    glm::ivec3 _playerChunk;
    std::vector<BlockData> _blocksData;
    std::vector<std::vector<BlockData*>> _visibleBlocks;
    std::vector<int> _renderedChunks;
    std::shared_ptr<InstanceRenderer> _sandRendererRef;
    int _sandRendererRefID;
    std::shared_ptr<InstanceRenderer> _topLayerRendererRef;
    int _topLayerRendererRefID;
    std::shared_ptr<InstanceRenderer> _plasticRendererRef;
    int _plasticRendererRefID;
    std::shared_ptr<InstanceRenderer> _metalRendererRef;
    int _metalRendererRefID;
    std::shared_ptr<Camera> _cameraRef;
    int _cameraRefID;
    std::vector<std::vector<glm::ivec3>> _sphereVectors;
    std::vector<glm::vec3> _topLayerPositions;

    void UpdateInstanceRenderer();
    void RefreshVisibleBlocks();
    void RefreshVisibleBlocks(int chunkIndex);
    void UpdateBlocksVisibility();
    void UpdateRenderedChunks();
    void DamageBlocks(glm::ivec3 hitPos, int radius, float digPower);
    void HideEdges();
    void UpdateBlockVisibility(BlockData& blockData);
    void UpdateNeighbourVisibility(BlockData& blockData);
    void UpdateNeighbourVisibility(BlockData& blockData, bool state);
    void SetVisibility(BlockData& blockData, bool state);
    void GenerateSphereVectors(int radius);
    std::vector<CollisionInfo> CalculateCollisionInfo(glm::vec3 entityPos, glm::vec3 movementVector, float halfWidth, float entityHeight);
    void GenerateMap(float initialFillRatio, int numIterations);
    void GenerateTopLayer(glm::ivec2 center, glm::ivec2 dimensions, glm::ivec2 deadzone);
    void InitializeMap(float initialFillRatio);
    void IterateCaveGeneration();
    void CheckEntityChunk(glm::vec3 entityPos);
    void ChangeType(BlockData& blockData, BlockType type);
    void ApplyMask(glm::ivec3 startPos, char* maskArray, glm::ivec3 maskDimensions);
    void GenerateResources();
    std::vector<glm::vec3> GeneratePoissonDiskPoints();
    void UpdateVisibilityNearResources();
    int DestroyBlock(BlockData& blockData);
    void ApplyMasks();
    void GenerateTunnel(std::vector<glm::ivec3> points, int size);
    void GenerateTunnels();
    void InitLayerStats();
    glm::ivec3 QuadraticBezier(const glm::ivec3& p0, const glm::ivec3& p1, const glm::ivec3& p2, float t);
    glm::ivec3 GetRandomSideBlock(glm::ivec2 yRange);
    glm::ivec3 GetRandomSideBlock(glm::ivec2 yRange, glm::ivec3 exclude);

    int GetMaterialReward(BlockData& blockData);
    float GetBlockHP(BlockData& blockData);
    float GetBlockHP(float y);
    int GetIndex(glm::ivec3 point);
    int GetIndex(int x, int y, int z);
    int GetChunkIndex(glm::ivec3 point);
    int GetChunkIndex(int x, int y, int z);
    bool CheckAdjacency(int x, int y, int z);
    bool InBounds(glm::ivec3 position);
    bool InBounds(int x, int y, int z);
    bool InBounds(glm::ivec3 position, int margin);
    bool ChunkInBounds(glm::ivec3 position);
    bool IsEdgeBlock(int x, int y, int z);
    bool IsEdgeBlock(glm::ivec3 pos);
    bool IsEdgeBlock(BlockData& blockData);
    bool IsPointTooClose(const std::vector<glm::vec3>& points, const glm::vec3& candidate, float minDist);
    BlockData* GetBlock(glm::ivec3 position);

    char _entranceMask[288] = {
    1,1,0,0,1,1,
    1,0,0,0,0,1,
    0,0,0,0,0,0,
    0,0,0,0,0,0,
    1,0,0,0,0,1,
    1,1,0,0,1,1,

    1,1,0,0,1,1,
    1,0,0,0,0,1,
    0,0,0,0,0,0,
    0,0,0,0,0,0,
    1,0,0,0,0,1,
    1,1,0,0,1,1,

    1,1,1,1,1,1,
    1,1,0,0,1,1,
    1,0,0,0,0,1,
    1,0,0,0,0,1,
    1,1,0,0,1,1,
    1,1,1,1,1,1,

    1,1,1,1,1,1,
    1,1,0,0,1,1,
    1,0,0,0,0,1,
    1,0,0,0,0,1,
    1,1,0,0,1,1,
    1,1,1,1,1,1,

    1,1,1,1,1,1,
    1,1,0,0,1,1,
    1,0,0,0,0,1,
    1,0,0,0,0,1,
    1,1,0,0,1,1,
    1,1,1,1,1,1,

    1,1,1,1,1,1,
    1,1,0,0,1,1,
    1,0,0,0,0,1,
    1,0,0,0,0,1,
    1,1,0,0,1,1,
    1,1,1,1,1,1,

    1,1,1,1,1,1,
    1,1,0,0,1,1,
    1,0,0,0,0,1,
    1,0,0,0,0,1,
    1,1,0,0,1,1,
    1,1,1,1,1,1,

    1,1,1,1,1,1,
    1,1,0,0,1,1,
    1,0,0,0,0,1,
    1,0,0,0,0,1,
    1,1,0,0,1,1,
    1,1,1,1,1,1 };
    glm::ivec3 _entranceMaskDimensions = glm::ivec3(6, 8, 6);

    char _tutorialCaveMask[7200] = {
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //4
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //5
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //6
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //6
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //7
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 0,0,0,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 0,0,0,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 0,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //8
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 1,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //9
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,1,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,1,0,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,0,1,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,1,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,0,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //10
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,1,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //11
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //12
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //13
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,0,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,0, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 4,0,0,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //14
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,5,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,5,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,4,0,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 4,0,0,0,0, 0,0,0,0,0, 0,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 0,0,0,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //15
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,0,5,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,0,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,0,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,4,0,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,0,0,0,0, 0,0,0,0,0, 1,1,1,1,1,
        1,1,1,1,1, 1,0,0,0,0, 0,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,4,0,0,0, 0,1,1,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,1,0,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //16
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,5,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,5, 0,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,0,0,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 0,0,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,0,0, 0,0,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 0,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,0, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //17
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //18
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //19
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,

        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, //20
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1,
        1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1, 1,1,1,1,1};
    glm::ivec3 _tutorialCaveMaskDimensions = glm::ivec3(20, 17, 20);
};
