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
    RefreshVisibleBlocks();
    UpdateInstanceRenderer();
    GenerateSphereVectors(31);
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
                BlockData sandBlock(BlockType::SAND, glm::ivec3(x, y, z), transformMatrix, 1.0f, false, shared_from_this());
                _blocksData.push_back(sandBlock);
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
            // Remove the block if it's either empty or not visible
            return blockPtr->GetBlockType() == BlockType::EMPTY || !blockPtr->IsVisible();
        }), _visibleBlocks.end());
}

void BlockManager::UpdateBlocksVisibility() {
    _visibleBlocks.clear(); // Clear the list of visible blocks

    // Iterate through all blocks
    for (auto& blockData : _blocksData) {
        UpdateBlockVisibility(blockData);
    }

    HideEdges();
}

void BlockManager::HideEdges()
{
    for (auto& blockData : _visibleBlocks) {
        if (blockData->GetPosID().y == 0) {
            blockData->SetVisible(false);
        }
        else if (blockData->GetPosID().y != _height - 1) {
            if (blockData->GetPosID().x == 0 || blockData->GetPosID().x == _width - 1) blockData->SetVisible(false);
            else if (blockData->GetPosID().z == 0 || blockData->GetPosID().z == _depth - 1) blockData->SetVisible(false);
        }
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

    if (state && blockData.GetBlockType() != BlockType::EMPTY) {
        // Add the blockData to _visibleBlocks
        _visibleBlocks.push_back(&blockData);
    }
}

void BlockManager::GenerateSphereVectors(int radius) {
    // Clear existing vectors
    _sphereVectors.clear();

    // Generate sphere vectors for each radius up to the specified radius
    for (int r = 0; r <= radius; r++) {
        std::vector<glm::ivec3> sphereVectors;

        // Iterate through each point within the cube that contains the sphere
        for (int x = -r; x <= r; x++) {
            for (int y = -r; y <= r; y++) {
                for (int z = -r; z <= r; z++) {
                    // Check if the distance from the center is within the radius
                    if (glm::length(glm::vec3(x, y, z)) <= r) {
                        sphereVectors.push_back(glm::ivec3(x, y, z));
                    }
                }
            }
        }

        // Add the generated vectors to _sphereVectors
        _sphereVectors.push_back(sphereVectors);
    }
}

bool BlockManager::RayIntersectsBlock(float rayLength, int radius) {
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
        if (InBounds(roundedPoint)) {

            // Calculate the index
            int index = GetIndex(roundedPoint);

            // Check if the blockData is visible and change it to empty if so
            if (_blocksData[index].IsVisible() && _blocksData[index].GetBlockType() != BlockType::EMPTY) {
                DamageBlocks(roundedPoint,radius);
                return true; // Block hit, no need to check further
            }
        }
    }
    return false; // No block hit along the ray
}

void BlockManager::DamageBlocks(glm::ivec3 hitPos, int radius)
{
    for (const glm::ivec3& offset : _sphereVectors[radius]) {
        glm::ivec3 pos = hitPos + offset;
        if (InBounds(pos)) {
            int index = GetIndex(pos);
            if (_blocksData[index].GetBlockType() != BlockType::EMPTY) {
                _blocksData[index].SetBlockType(BlockType::EMPTY);
                UpdateNeighbourVisibility(_blocksData[index]);
            }
        }
    }

    RefreshVisibleBlocks();
    UpdateInstanceRenderer();
}

std::vector<CollisionInfo> BlockManager::CalculateCollisionInfo(glm::vec3 entityPos, glm::vec3 movementVector, float halfWidth, float entityHeight) {
    std::vector<CollisionInfo> collisionInfoList;

    // Calculate the positions of the corners of the entity's AABB
    glm::vec3 corners[12];
    corners[0] = entityPos + glm::vec3(-halfWidth, 0.0f, -halfWidth);
    corners[1] = entityPos + glm::vec3(halfWidth, 0.0f, -halfWidth);
    corners[2] = entityPos + glm::vec3(-halfWidth, 0.0f, halfWidth);
    corners[3] = entityPos + glm::vec3(halfWidth, 0.0f, halfWidth);
    corners[4] = entityPos + glm::vec3(-halfWidth, 1.0f, -halfWidth);
    corners[5] = entityPos + glm::vec3(halfWidth, 1.0f, -halfWidth);
    corners[6] = entityPos + glm::vec3(-halfWidth, 1.0f, halfWidth);
    corners[7] = entityPos + glm::vec3(halfWidth, 1.0f, halfWidth);
    corners[8] = entityPos + glm::vec3(-halfWidth, entityHeight, -halfWidth);
    corners[9] = entityPos + glm::vec3(halfWidth, entityHeight, -halfWidth);
    corners[10] = entityPos + glm::vec3(-halfWidth, entityHeight, halfWidth);
    corners[11] = entityPos + glm::vec3(halfWidth, entityHeight, halfWidth);

    // Iterate through the lower corners (0-3) and check collision with the nearest blocks below
    for (int i = 0; i < 12; i++) {
        // Round the position of the corner to get the ID of the nearest block below
        glm::ivec3 roundedPosX = glm::round(glm::vec3(corners[i].x + movementVector.x, corners[i].y,corners[i].z));
        glm::ivec3 roundedPosY = glm::round(glm::vec3(corners[i].x, corners[i].y + movementVector.y, corners[i].z));
        glm::ivec3 roundedPosZ = glm::round(glm::vec3(corners[i].x , corners[i].y, corners[i].z + movementVector.z));

        // Initialize CollisionInfo for the current corner
        CollisionInfo info;

        // Check if the rounded position is within bounds
        if (InBounds(roundedPosX)) {
            int index = GetIndex(roundedPosX);
            
            // Check if the block at the index is not empty
            if (_blocksData[index].GetBlockType() != BlockType::EMPTY) {
                // Calculate the AABB extents of the block
                glm::vec3 blockMin = glm::vec3(roundedPosX) - glm::vec3(0.5f);
                glm::vec3 blockMax = blockMin + glm::vec3(1.0f);

                float entityPosX = entityPos.x + movementVector.x;
                if (entityPosX + halfWidth > blockMin.x || entityPosX - halfWidth < blockMax.x) {
                    // Determine the separation vector in the x-axis
                    float direction = glm::sign(entityPosX - roundedPosX.x);
                    info.separationVector.x = (std::abs((std::abs(entityPosX - roundedPosX.x) - (halfWidth + 0.5f))) + 0.00001f) * direction;
                    info.isColliding = true;
                }
            }
        }

        if (InBounds(roundedPosY)) {
            int index = GetIndex(roundedPosY);

            // Check if the block at the index is not empty
            if (_blocksData[index].GetBlockType() != BlockType::EMPTY) {
                // Calculate the AABB extents of the block
                glm::vec3 blockMin = glm::vec3(roundedPosY) - glm::vec3(0.5f);
                glm::vec3 blockMax = blockMin + glm::vec3(1.0f);

                float entityPosY = entityPos.y + movementVector.y;
                if (i < 4) {
                    if (entityPosY < blockMax.y && entityPosY > roundedPosY.y) {
                        info.separationVector.y = (std::abs((std::abs(entityPosY - blockMax.y))) + 0.00001f);
                        info.isColliding = true;
                    }
                }
                else if (i >= 8) {
                    if (entityPosY + entityHeight > blockMin.y && entityPosY + entityHeight < roundedPosY.y) {
                        info.separationVector.y = (std::abs((std::abs(entityPosY + entityHeight - blockMin.y))) + 0.00001f) * -1;
                        info.isColliding = true;
                    }
                }
            }
        }

        if (InBounds(roundedPosZ)) {
            int index = GetIndex(roundedPosZ);

            // Check if the block at the index is not empty
            if (_blocksData[index].GetBlockType() != BlockType::EMPTY) {
                // Calculate the AABB extents of the block
                glm::vec3 blockMin = glm::vec3(roundedPosZ) - glm::vec3(0.5f);
                glm::vec3 blockMax = blockMin + glm::vec3(1.0f);

                float entityPosZ = entityPos.z + movementVector.z;
                if (entityPosZ + halfWidth > blockMin.z || entityPosZ - halfWidth < blockMax.z) {
                    // Determine the separation vector in the z-axis
                    float direction = glm::sign(entityPosZ - roundedPosZ.z);
                    info.separationVector.z = (std::abs((std::abs(entityPosZ - roundedPosZ.z) - (halfWidth + 0.5f))) + 0.00001f) * direction;
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

std::pair<glm::vec3,glm::vec3> BlockManager::CheckEntityCollision(glm::vec3 entityPos, glm::vec3 movementVector, float entityWidth, float entityHeight) {
    // Calculate the half extents of the entity's AABB
    float halfWidth = entityWidth / 2.0f;

    // Calculate collision information for each corner of the entity's AABB
    std::vector<CollisionInfo> collisionInfoList = CalculateCollisionInfo(entityPos, movementVector, halfWidth, entityHeight);

    float smallestX = std::numeric_limits<float>::max();
    float smallestY = std::numeric_limits<float>::max();
    float smallestZ = std::numeric_limits<float>::max();

    for (const CollisionInfo& collisionInfo : collisionInfoList) {
        // Check if collision occurred
        if (collisionInfo.isColliding) {
            // Update separationVector based on the smallest non-zero component
            if (std::abs(collisionInfo.separationVector.x) < std::abs(collisionInfo.separationVector.z)) {
                if(std::abs(collisionInfo.separationVector.x) < std::abs(smallestX))
                    smallestX = collisionInfo.separationVector.x;
            }
            else if (collisionInfo.separationVector.z < std::numeric_limits<float>::max()) {
                if (std::abs(collisionInfo.separationVector.z) < std::abs(smallestZ))
                    smallestZ = collisionInfo.separationVector.z;
            }

            if (collisionInfo.separationVector.y < std::numeric_limits<float>::max()) {
                if (std::abs(collisionInfo.separationVector.y) < std::abs(smallestY))
                    smallestY = collisionInfo.separationVector.y;
            }
        }
    }

    glm::vec3 separationVector = glm::vec3(smallestX, smallestY, smallestZ);

    // If any component of separationVector is still float max, set it to 0.0f
    if (separationVector.x == std::numeric_limits<float>::max()) {
        separationVector.x = 0.0f;
    }
    if (separationVector.y == std::numeric_limits<float>::max()) {
        separationVector.y = 0.0f;
    }
    if (separationVector.z == std::numeric_limits<float>::max()) {
        separationVector.z = 0.0f;
    }

    //std::cout << "Mov: " << movementVector.y << " | SepX : " << separationVector.x << " | SepY : " << separationVector.y << "  | SepZ:" << separationVector.z <<  " | EposY : " << entityPos.y << std::endl;
    movementVector += separationVector;

    // Return the separation vector
    return std::make_pair(movementVector,separationVector);
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
