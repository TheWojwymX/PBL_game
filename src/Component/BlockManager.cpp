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

    // Iterate through _blocksData and add visible non-empty blocks to instanceMatrix
    for (const auto& blockTuple : _blocksData) {
        const BlockData& blockData = std::get<0>(blockTuple);
        if (blockData.IsVisible() && blockData.GetBlockType() != BlockType::EMPTY) {
            glm::mat4 transformMatrix = std::get<1>(blockTuple);
            instanceMatrix.push_back(transformMatrix);
        }
    }

    // Pass the instanceMatrix to _sandRendererRef
    if (_sandRendererRef) {
        _sandRendererRef->SetInstanceMatrix(instanceMatrix);
    }
}

void BlockManager::UpdateBlocksVisibility() {
    _visibleBlocks.clear(); // Clear the list of visible blocks

    // Iterate through all blocks
    for (auto& blockTuple : _blocksData) {
        BlockData& blockData = std::get<0>(blockTuple);
        UpdateBlockVisibility(blockData);

        // Add visible non-empty blocks to _visibleBlocks
        if (blockData.IsVisible() && blockData.GetBlockType() != BlockType::EMPTY) {
            _visibleBlocks.push_back(&blockData);
        }
    }
}

void BlockManager::UpdateBlockVisibility(BlockData& blockData) {
    // Get the position of the current block
    int x = blockData.GetXID();
    int y = blockData.GetYID();
    int z = blockData.GetZID();

    // Check adjacent blocks for emptiness
    bool leftBlockEmpty = x - 1 >= 0 && CheckAdjacency(x - 1, y, z);
    bool rightBlockEmpty = x + 1 < _width && CheckAdjacency(x + 1, y, z);
    bool topBlockEmpty = y + 1 < _height && CheckAdjacency(x, y + 1, z);
    bool bottomBlockEmpty = y - 1 >= 0 && CheckAdjacency(x, y - 1, z);
    bool frontBlockEmpty = z + 1 < _depth && CheckAdjacency(x, y, z + 1);
    bool backBlockEmpty = z - 1 >= 0 && CheckAdjacency(x, y, z - 1);

    // If any adjacent block is absent or not empty, set visibility to true
    if (!leftBlockEmpty || !rightBlockEmpty || !topBlockEmpty || !bottomBlockEmpty || !frontBlockEmpty || !backBlockEmpty) {
        blockData.SetVisible(true);
    }
    else {
        blockData.SetVisible(false);
    }
}


bool BlockManager::RayIntersectsBlock(float rayLength) {
    // Get camera position and front vector
    glm::vec3 cameraPos = _cameraRef->GetPosition();
    glm::vec3 cameraFront = _cameraRef->GetFrontVector();

    // Iterate through points along the ray at 0.5 * direction steps
    for (float t = 0.0f; t <= rayLength; t += 0.5f) {
        // Calculate the point along the ray
        glm::vec3 point = cameraPos + t * cameraFront;

        // Round the point to whole numbers
        glm::ivec3 roundedPoint = glm::round(point);
        // Check if all three elements of roundedPoint are positive
        if (roundedPoint.x >= 0 && roundedPoint.y >= 0 && roundedPoint.z >= 0) {
            // Calculate the index
            int index = GetIndex(roundedPoint);
            // Check if the index is within bounds
            if (index >= 0 && index < _blocksData.size()) {
                // Check if the blockData is visible and change it to empty if so
                if (std::get<0>(_blocksData[index]).IsVisible() && std::get<0>(_blocksData[index]).GetBlockType() != BlockType::EMPTY) {
                    std::get<0>(_blocksData[index]).SetBlockType(BlockType::EMPTY);
                    UpdateInstanceRenderer();
                    return true; // Block hit, no need to check further
                }
            }
        }
    }
    return false; // No block hit along the ray
}

int BlockManager::GetIndex(glm::ivec3 point) {
    return point.x + (_width * _depth * point.y) + point.z * _width;
}

int BlockManager::GetIndex(int x, int y, int z) {
    return x + (_width * _depth * y) + z * _width;
}

bool BlockManager::CheckAdjacency(int x, int y, int z)
{
    return std::get<0>(_blocksData[GetIndex(x, y, z)]).GetBlockType() != BlockType::EMPTY;
}

