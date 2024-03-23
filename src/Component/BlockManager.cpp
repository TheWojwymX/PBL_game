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

                // Create BlockData object with Sand type and add it to the vector
                BlockData blockData(BlockType::SAND, glm::ivec3(x, y, z), transformMatrix, 1.0f, false, shared_from_this());
                _blocksData.push_back(blockData);
            }
        }
    }
}

void BlockManager::UpdateInstanceRenderer() {
    std::vector<glm::mat4> instanceMatrix;

    // Iterate through _visibleBlocks and add visible non-empty blocks to instanceMatrix
    for (const auto& blockPtr : _visibleBlocks) {
        const BlockData& blockData = *blockPtr;

        glm::mat4 transformMatrix = blockData.GetMatrix();
        instanceMatrix.push_back(transformMatrix);
    }

    // Pass the instanceMatrix to _sandRendererRef
    if (_sandRendererRef) {
        _sandRendererRef->SetInstanceMatrix(instanceMatrix);
    }
}

void BlockManager::RefreshVisibleBlocks() {
    // Use erase-remove idiom to remove elements satisfying the condition
    _visibleBlocks.erase(std::remove_if(_visibleBlocks.begin(), _visibleBlocks.end(),
        [](BlockData* blockPtr) {
            return blockPtr->GetBlockType() == BlockType::EMPTY;
        }), _visibleBlocks.end());
}


void BlockManager::UpdateBlocksVisibility() {
    _visibleBlocks.clear(); // Clear the list of visible blocks

    // Iterate through all blocks
    for (auto& blockData : _blocksData) {
        UpdateBlockVisibility(blockData);
    }
}

void BlockManager::UpdateNeighbourVisibility(BlockData& blockData)
{
    glm::ivec3 posID = blockData.GetPosID();
    int x = posID.x;
    int y = posID.y;
    int z = posID.z;

    // Update visibility for neighboring blocks

    if (x - 1 >= 0 && x - 1 < _width) SetVisibility(_blocksData[GetIndex(x - 1, y, z)], true); // Left
    if (x + 1 >= 0 && x + 1 < _width) SetVisibility(_blocksData[GetIndex(x + 1, y, z)], true); // Right
    if (y + 1 >= 0 && y + 1 < _height) SetVisibility(_blocksData[GetIndex(x, y + 1, z)], true); // Top
    if (y - 1 >= 0 && y - 1 < _height) SetVisibility(_blocksData[GetIndex(x, y - 1, z)], true); // Bottom
    if (z + 1 >= 0 && z + 1 < _depth) SetVisibility(_blocksData[GetIndex(x, y, z + 1)], true); // Front
    if (z - 1 >= 0 && z - 1 < _depth) SetVisibility(_blocksData[GetIndex(x, y, z - 1)], true); // Back
}

void BlockManager::UpdateBlockVisibility(BlockData& blockData)
{
    // Get the position of the current block
    glm::ivec3 posID = blockData.GetPosID();
    int x = posID.x;
    int y = posID.y;
    int z = posID.z;

    // Check adjacent blocks for emptiness
    bool leftBlockEmpty = x - 1 >= 0 && CheckAdjacency(x - 1, y, z);
    bool rightBlockEmpty = x + 1 < _width && CheckAdjacency(x + 1, y, z);
    bool topBlockEmpty = y + 1 < _height && CheckAdjacency(x, y + 1, z);
    bool bottomBlockEmpty = y - 1 >= 0 && CheckAdjacency(x, y - 1, z);
    bool frontBlockEmpty = z + 1 < _depth && CheckAdjacency(x, y, z + 1);
    bool backBlockEmpty = z - 1 >= 0 && CheckAdjacency(x, y, z - 1);

    // Check if the block will be visible
    bool isVisible = !leftBlockEmpty || !rightBlockEmpty || !topBlockEmpty ||
        !bottomBlockEmpty || !frontBlockEmpty || !backBlockEmpty;

    // Set the visibility of the block
    SetVisibility(blockData, isVisible);
}


void BlockManager::SetVisibility(BlockData& blockData, bool state) {
    // Set the visibility of the block
    blockData.SetVisible(state);

    // If the block is visible and not empty, add it to _visibleBlocks
    if (state && blockData.GetBlockType() != BlockType::EMPTY) {
        _visibleBlocks.push_back(&blockData);
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
                if (_blocksData[index].IsVisible() && _blocksData[index].GetBlockType() != BlockType::EMPTY) {
                    _blocksData[index].SetBlockType(BlockType::EMPTY);
                    UpdateNeighbourVisibility(_blocksData[index]);
                    RefreshVisibleBlocks();
                    UpdateInstanceRenderer();
                    return true; // Block hit, no need to check further
                }
            }
        }
    }
    return false; // No block hit along the ray
}

bool BlockManager::CheckEntityCollision(const glm::vec3& entityPos, float entityWidth, float entityHeight) {
    // Round the entity position to integers
    glm::ivec3 roundedEntityPos = glm::round(entityPos);

    // Check if the rounded entity position is within bounds
    if (roundedEntityPos.x < 0 || roundedEntityPos.y < 0 || roundedEntityPos.z < 0 ||
        roundedEntityPos.x >= _width || roundedEntityPos.y >= _height || roundedEntityPos.z >= _depth) {
        return false; // Out of bounds
    }

    // Calculate the index of the block below the entity
    int indexBelowEntity = GetIndex(roundedEntityPos.x, roundedEntityPos.y - 1, roundedEntityPos.z);

    // Check if the block below the entity is not empty
    if (_blocksData[indexBelowEntity].GetBlockType() != BlockType::EMPTY) 
        return true; // Collision detected

    return false; // No collision
}

int BlockManager::GetIndex(glm::ivec3 point) {
    return point.x + (_width * _depth * point.y) + point.z * _width;
}

int BlockManager::GetIndex(int x, int y, int z) {
    // Check if the provided coordinates are within bounds
    if (x >= 0 && x < _width && y >= 0 && y < _height && z >= 0 && z < _depth) {
        // Calculate the index using the provided coordinates
        return x + (_width * _depth * y) + z * _width;
    } else {
        return 0; // Lol hardcoded out-of-range prevention
    }
}


bool BlockManager::CheckAdjacency(int x, int y, int z)
{
    return _blocksData[GetIndex(x, y, z)].GetBlockType() != BlockType::EMPTY;
}
