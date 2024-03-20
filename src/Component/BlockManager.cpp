#include "BlockManager.h"

BlockManager::BlockManager(int width, int height, int depth) :
    _width(width), _depth(depth), _height(height) {}

void BlockManager::Init() {
    GenerateMap();
    UpdateInstanceRenderer();
}

void BlockManager::GenerateMap() {
    _blockData.clear();

    // Iterate over the grid dimensions
    for (int y = 0; y < _height; y++) {
        for (int z = 0; z < _depth; z++) {
            for (int x = 0; x < _width; x++) {
                // Calculate transform matrix for the current block
                glm::mat4 transformMatrix = Transform::CalculateTransformMatrix(glm::vec3(x, y, z), glm::quat(), glm::vec3(1.0f));

                // Create BlockData object with Sand type and add it to the vector along with the transform matrix
                _blockData.emplace_back(BlockData(BlockType::SAND, x, y, z, 1.0f, false, shared_from_this()), transformMatrix);
            }
        }
    }
}

void BlockManager::UpdateInstanceRenderer() {
    std::vector<glm::mat4> instanceMatrix;

    // Iterate through _blockData and add visible blocks to instanceMatrix
    for (const auto& blockTuple : _blockData) {
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

void BlockManager::SetInstanceRenderer(std::shared_ptr<InstanceRenderer> renderer) {
    _sandRendererRef = renderer;
}
