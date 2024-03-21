#include "BlockManager.h"

BlockManager::BlockManager(int width, int height, int depth) :
    _width(width), _depth(depth), _height(height) {}

void BlockManager::Init() {
    GenerateMap();
    UpdateBlocksVisibility();
    UpdateInstanceRenderer();
}

void BlockManager::GenerateMap() {
    _blocksData.clear();

    // Iterate over the grid dimensions
    for (int y = 0; y < _height; y++) {
        for (int z = 0; z < _depth; z++) {
            for (int x = 0; x < _width; x++) {
                // Calculate transform matrix for the current block
                glm::mat4 transformMatrix = Transform::CalculateTransformMatrix(glm::vec3(x, y, z), glm::quat(), glm::vec3(1.0f));

                // Create BlockData object with Sand type and add it to the vector along with the transform matrix
                _blocksData.emplace_back(BlockData(BlockType::SAND, x, y, z, 1.0f, false, shared_from_this()), transformMatrix);
            }
        }
    }
}

void BlockManager::UpdateInstanceRenderer() {
    std::vector<glm::mat4> instanceMatrix;

    // Iterate through _blockData and add visible blocks to instanceMatrix
    for (const auto& blockTuple : _blocksData) {
        const BlockData& blockData = std::get<0>(blockTuple);
        if (blockData.IsVisible()) {
            glm::mat4 transformMatrix = std::get<1>(blockTuple);
            instanceMatrix.push_back(transformMatrix);
        }
    }

    // Pass the instanceMatrix to _instanceRendererRef
    if (_sandRendererRef) {
        _sandRendererRef->SetInstanceMatrix(instanceMatrix);
    }
}

void BlockManager::UpdateBlocksVisibility() {
    // Iterate through all blocks
    for (auto& blockTuple : _blocksData) {
        BlockData& blockData = std::get<0>(blockTuple);
        UpdateBlockVisibility(blockData);
    }
}

void BlockManager::UpdateBlockVisibility(BlockData& blockData) {
    // Get the position of the current block
    int x = blockData.GetXID();
    int y = blockData.GetYID();
    int z = blockData.GetZID();

    // Check visibility based on adjacent blocks
    bool leftBlock = x - 1 >= 0 && std::get<0>(_blocksData[(x - 1) * _width + y * _height + z * _depth]).IsVisible();
    bool rightBlock = x + 1 < _width && std::get<0>(_blocksData[(x + 1) * _width + y * _height + z * _depth]).IsVisible();
    bool topBlock = y + 1 < _height && std::get<0>(_blocksData[x * _width + (y + 1) * _height + z * _depth]).IsVisible();
    bool bottomBlock = y - 1 >= 0 && std::get<0>(_blocksData[x * _width + (y - 1) * _height + z * _depth]).IsVisible();
    bool frontBlock = z + 1 < _depth && std::get<0>(_blocksData[x * _width + y * _height + (z + 1) * _depth]).IsVisible();
    bool backBlock = z - 1 >= 0 && std::get<0>(_blocksData[x * _width + y * _height + (z - 1) * _depth]).IsVisible();

    // If any adjacent block is absent or not empty, set visibility to true
    if (!leftBlock || !rightBlock || !topBlock || !bottomBlock || !frontBlock || !backBlock) {
        blockData.SetVisible(true);
    }
    else {
        blockData.SetVisible(false);
    }
}

void BlockManager::SetInstanceRenderer(std::shared_ptr<InstanceRenderer> renderer) {
    _sandRendererRef = renderer;
}
