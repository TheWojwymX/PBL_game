#pragma once

#include <vector>
#include <memory>
#include <algorithm>
#include <random>
#include <cmath>
#include "Core/Component.h"
#include "Component/InstanceRenderer.h"
#include "Managers/ComponentsManager.h"
#include "Component/Camera.h"
#include "BlockData.h"
#include "imgui.h"

class CloudManager : public Component, public std::enable_shared_from_this<CloudManager> {
public:
    CloudManager(int width, int height, int depth);

    CloudManager();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json& jsonData) override;

    void initiate() override;

    void Init() override;

    void Refresh();

    bool RayIntersectsBlock(float rayLength, int radius);

    void SetInstanceRenderer(std::shared_ptr<InstanceRenderer> renderer) { _sandRendererRef = renderer; }
    void SetCamera(std::shared_ptr<Camera> camera) { _cameraRef = camera; }

    void addToInspector(ImguiMain *imguiMain) override;

    static CloudManager& GetInstance() {
        static CloudManager instance; // Static instance of CloudManager
        return instance;
    }

private:
    int _width;
    int _depth;
    int _height;
    std::vector<BlockData> _blocksData;
    std::vector<BlockData*> _visibleBlocks;
    std::shared_ptr<InstanceRenderer> _sandRendererRef;
    std::shared_ptr<Camera> _cameraRef;
    std::vector<std::vector<glm::ivec3>> _sphereVectors;
    int _sandRendererRefID;
    int _cameraRefID;


    void GenerateMap();
    void UpdateInstanceRenderer();
    void RefreshVisibleBlocks();
    void UpdateBlocksVisibility();
    void DamageBlocks(glm::ivec3 hitPos, int radius);
    void HideEdges();
    void UpdateBlockVisibility(BlockData& blockData);
    void UpdateNeighbourVisibility(BlockData& blockData);
    void SetVisibility(BlockData& blockData, bool state);
    void GenerateSphereVectors(int radius);
    void MoveAllBlocks(const glm::vec3& offset);
    void MoveBlocksAsPath(const glm::vec3& offset);
    void Update();
    float Perlin(float x, float y, float z);
    void SetDensity(int x, int y, int z, float density);

    int GetIndex(glm::ivec3 point);
    int GetIndex(int x, int y, int z);
    bool CheckAdjacency(int x, int y, int z);
    bool InBounds(glm::ivec3 position);
    float RandomInRange(float min, float max);
};