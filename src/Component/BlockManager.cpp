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

void BlockManager::HitBlock() {
    // Get camera position and front vector
    glm::vec3 cameraPos = _cameraRef->GetPosition();
    glm::vec3 cameraFront = _cameraRef->GetFrontVector();
    std::cout << cameraFront.x << " " << cameraFront.y << " " << cameraFront.z << std::endl;

    // Calculate the ray direction (assuming normalized)
    glm::vec3 rayDirection = glm::normalize(cameraFront);
    std::cout << rayDirection.x << " " << rayDirection.y << " " << rayDirection.z << std::endl;
    // Iterate through visible blocks to check for hits
    for (auto& blockData : _visibleBlocks) {
        glm::vec3 blockPos = glm::vec3(blockData->GetXID(), blockData->GetYID(), blockData->GetZID());

        // Check if the block is hit by the ray
        if (RayIntersectsBlock(cameraPos, rayDirection, blockPos)) {
            // Destroy the block by changing its state to EMPTY
            blockData->SetBlockType(BlockType::EMPTY);

            // Update the instance renderer to reflect the changes
            UpdateInstanceRenderer();
            break; // Once a block is destroyed, stop checking for hits
        }
    }
}



bool BlockManager::RayIntersectsBlock(const glm::vec3& origin, const glm::vec3& direction, const glm::vec3& blockPos) {
    // Box parameters
    glm::vec3 boxMin = blockPos - glm::vec3(0.5f, 0.5f, 0.5f); // Assuming the block position is its center
    glm::vec3 boxMax = blockPos + glm::vec3(0.5f, 0.5f, 0.5f);

    // Adjust origin to ensure it's inside or very close to the block
    glm::vec3 adjustedOrigin = origin;
    if (direction.x < 0) adjustedOrigin.x += 0.5f;
    if (direction.y < 0) adjustedOrigin.y += 0.5f;
    if (direction.z < 0) adjustedOrigin.z += 0.5f;

    // Calculate parameters for AABB intersection tests
    glm::vec3 invDirection = 1.0f / direction;
    glm::vec3 tMin = (boxMin - adjustedOrigin) * invDirection;
    glm::vec3 tMax = (boxMax - adjustedOrigin) * invDirection;

    // Ensure correct order of tMin and tMax
    glm::vec3 t1 = glm::min(tMin, tMax);
    glm::vec3 t2 = glm::max(tMin, tMax);

    // Get the largest minimum value and smallest maximum value along each axis
    float largestMin = std::max(std::max(t1.x, t1.y), t1.z);
    float smallestMax = std::min(std::min(t2.x, t2.y), t2.z);

    // Check if the ray intersects with the box
    return smallestMax > largestMin && largestMin > 0.0f;
}
