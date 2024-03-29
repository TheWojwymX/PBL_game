#include "BlockManager.h"

BlockManager::BlockManager(int width, int height, int depth) :
    _width(width), _depth(depth), _height(height) {}

BlockManager::BlockManager() {

}

nlohmann::json BlockManager::Serialize() {
    nlohmann::json data = Component::Serialize();

    data["type"] = "BlockManager";
    data["width"] = _width;
    data["depth"] = _depth;
    data["height"] = _height;

    return data;
}

void BlockManager::Deserialize(const nlohmann::json &jsonData) {

    if (jsonData.contains("width")) {
        _width = jsonData["width"].get<int>();
    }

    if (jsonData.contains("depth")) {
        _depth = jsonData["depth"].get<int>();
    }

    if (jsonData.contains("height")) {
        _height = jsonData["height"].get<int>();
    }

    Component::Deserialize(jsonData);
}

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

    // Iterate through points along the ray at 0.25 * direction steps
    for (float t = 0.0f; t <= rayLength; t += 0.25f) {
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

// Function to calculate collision information for each corner of the entity's AABB
std::vector<CollisionInfo> BlockManager::CalculateCollisionInfo(const glm::vec3& entityPos, float halfWidth, float entityHeight) {
    std::vector<CollisionInfo> collisionInfoList;

    // Calculate the positions of the corners of the entity's AABB
    glm::vec3 corners[8];
    corners[0] = entityPos + glm::vec3(-halfWidth, 0.0f, -halfWidth);
    corners[1] = entityPos + glm::vec3(halfWidth, 0.0f, -halfWidth);
    corners[2] = entityPos + glm::vec3(-halfWidth, 0.0f, halfWidth);
    corners[3] = entityPos + glm::vec3(halfWidth, 0.0f, halfWidth);
    corners[4] = entityPos + glm::vec3(-halfWidth, entityHeight, -halfWidth);
    corners[5] = entityPos + glm::vec3(halfWidth, entityHeight, -halfWidth);
    corners[6] = entityPos + glm::vec3(-halfWidth, entityHeight, halfWidth);
    corners[7] = entityPos + glm::vec3(halfWidth, entityHeight, halfWidth);

    // Iterate through the corners and check collision with the nearest blocks
    for (int i = 0; i < 8; i++) {
        // Round the position of the corner to get the ID of the nearest block
        glm::ivec3 roundedPos = glm::round(corners[i]);

        // Initialize CollisionInfo for the current corner
        CollisionInfo info;
        info.cornerPosition = corners[i];
        info.isColliding = false;

        // Check if the rounded position is within bounds
        if (InBounds(roundedPos)) {
            // Calculate the index of the nearest block
            int index = GetIndex(roundedPos);

            // Check if the block at the index is not empty
            if (_blocksData[index].GetBlockType() != BlockType::EMPTY) {
                // Calculate the AABB extents of the block
                glm::vec3 blockMin = glm::vec3(roundedPos) - glm::vec3(0.5f);
                glm::vec3 blockMax = blockMin + glm::vec3(1.0f);

                if (entityPos.x + halfWidth > blockMin.x && entityPos.x - halfWidth < blockMax.x) {
                    // Determine the separation vector in the x-axis
                    float direction = glm::sign(entityPos.x - roundedPos.x);
                    info.separationVector.x = (std::abs((std::abs(entityPos.x - roundedPos.x) - (halfWidth + 0.5f)) + 0.0001f)) * direction;
                    info.isColliding = true;
                }

                if (entityPos.z + halfWidth > blockMin.z && entityPos.z - halfWidth < blockMax.z) {
                    // Determine the separation vector in the x-axis
                    float direction = glm::sign(entityPos.z - roundedPos.z);
                    info.separationVector.z = (std::abs((std::abs(entityPos.z - roundedPos.z) - (halfWidth + 0.5f)) + 0.0001f)) * direction;
                    info.isColliding = true;
                }
            }
        }

        // Add collision info to the list
        collisionInfoList.push_back(info);
    }

    // Return the list of collision information for each corner
    return collisionInfoList;
}


glm::vec3 BlockManager::CheckEntityCollision(const glm::vec3& entityPos, const glm::vec3& entityDirection, float entityWidth, float entityHeight) {
    // Calculate the half extents of the entity's AABB
    float halfWidth = entityWidth / 2.0f;

    // Calculate collision information for each corner of the entity's AABB
    std::vector<CollisionInfo> collisionInfoList = CalculateCollisionInfo(entityPos, halfWidth, entityHeight);

    // Initialize the separation vector
    glm::vec3 separationVector(0.0f);

    // Check if collisionInfoList[1] or collisionInfoList[5] has collision
    if (collisionInfoList[1].isColliding || collisionInfoList[5].isColliding) {
        // Determine which collision info to use
        const CollisionInfo& chosenCollisionInfo = collisionInfoList[1].isColliding ? collisionInfoList[1] : collisionInfoList[5];

        // Check which axis of separation is smaller
        if (std::abs(chosenCollisionInfo.separationVector.x) < std::abs(chosenCollisionInfo.separationVector.z)) {
            separationVector.x = chosenCollisionInfo.separationVector.x;
        }
        else {
            separationVector.z = chosenCollisionInfo.separationVector.z;
        }
    }
    // Check if collisionInfoList[3] or collisionInfoList[7] has collision
    if (collisionInfoList[3].isColliding || collisionInfoList[7].isColliding) {
        // Determine which collision info to use
        const CollisionInfo& chosenCollisionInfo = collisionInfoList[3].isColliding ? collisionInfoList[3] : collisionInfoList[7];

        // Check which axis of separation is smaller
        if (std::abs(chosenCollisionInfo.separationVector.x) < std::abs(chosenCollisionInfo.separationVector.z)) {
            separationVector.x = chosenCollisionInfo.separationVector.x;
        }
        else {
            separationVector.z = chosenCollisionInfo.separationVector.z;
        }
    }
    // Check if collisionInfoList[0] or collisionInfoList[4] has collision
    if (collisionInfoList[0].isColliding || collisionInfoList[4].isColliding) {
        // Determine which collision info to use
        const CollisionInfo& chosenCollisionInfo = collisionInfoList[0].isColliding ? collisionInfoList[0] : collisionInfoList[4];

        // Check which axis of separation is smaller
        if (std::abs(chosenCollisionInfo.separationVector.x) < std::abs(chosenCollisionInfo.separationVector.z)) {
            separationVector.x = chosenCollisionInfo.separationVector.x;
        }
        else {
            separationVector.z = chosenCollisionInfo.separationVector.z;
        }
    }
    // Check if collisionInfoList[2] or collisionInfoList[6] has collision
    if (collisionInfoList[2].isColliding || collisionInfoList[6].isColliding) {
        // Determine which collision info to use
        const CollisionInfo& chosenCollisionInfo = collisionInfoList[2].isColliding ? collisionInfoList[2] : collisionInfoList[6];

        // Check which axis of separation is smaller
        if (std::abs(chosenCollisionInfo.separationVector.x) < std::abs(chosenCollisionInfo.separationVector.z)) {
            separationVector.x = chosenCollisionInfo.separationVector.x;
        }
        else {
            separationVector.z = chosenCollisionInfo.separationVector.z;
        }
    }

    // Count the number of corners that have true in collision
    int numCollidingCorners = 0;
    //std::cout << "Colliding corners indices: ";
    for (size_t i = 0; i < collisionInfoList.size(); ++i) {
        if (collisionInfoList[i].isColliding) {
            //std::cout << i << " ";
            numCollidingCorners++;
        }
    }
    //std::cout << std::endl;

    // Print the number of colliding corners
    //std::cout << "Number of colliding corners: " << numCollidingCorners << std::endl;

    // Return the separation vector
    return separationVector;
}

int BlockManager::GetIndex(glm::ivec3 point) {
    return point.x + (_width * _depth * point.y) + point.z * _width;
}

int BlockManager::GetIndex(int x, int y, int z) {
    return x + (_width * _depth * y) + z * _width;
}


bool BlockManager::CheckAdjacency(int x, int y, int z)
{
    return _blocksData[GetIndex(x, y, z)].GetBlockType() != BlockType::EMPTY;
}

bool BlockManager::InBounds(glm::ivec3 position) {
    return position.x >= 0 && position.y >= 0 && position.z >= 0 &&
        position.x < _width && position.y < _height && position.z < _depth;
}

void BlockManager::addToInspector(ImguiMain *imguiMain)
{
    ImGui::Text(" Siema jestem blok manager ");
}
