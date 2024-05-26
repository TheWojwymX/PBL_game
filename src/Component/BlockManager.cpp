#include "BlockManager.h"

BlockManager::BlockManager(int width, int height, int depth) :
    _width(width), _depth(depth), _height(height), _chunkSize(20), _playerChunk(glm::ivec3(0.0f)), _renderDistance(3) {
}

BlockManager::BlockManager() : _chunkSize(20), _playerChunk(glm::ivec3(0.0f)), _renderDistance(3) {
    _type = ComponentType::BLOCKMANAGER;
}


nlohmann::json BlockManager::Serialize() {
    nlohmann::json data = Component::Serialize();

    data["width"] = _width;
    data["depth"] = _depth;
    data["height"] = _height;

    if(_sandRendererRef){
        data["sandRendererRefID"] = _sandRendererRef->_id;
    }

    if (_topLayerRendererRef) {
        data["topLayerRendererRef"] = _topLayerRendererRef->_id;
    }

    if(_cameraRef){
        data["cameraRefID"] = _cameraRef->_id;
    }

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

    if (jsonData.contains("sandRendererRefID")) {
        _sandRendererRefID = jsonData["sandRendererRefID"].get<int>();
    }

    if (jsonData.contains("topLayerRendererRefID")) {
        _sandRendererRefID = jsonData["topLayerRendererRefID"].get<int>();
    }

    if (jsonData.contains("cameraRefID")) {
        _cameraRefID = jsonData["cameraRefID"].get<int>();
    }

    Component::Deserialize(jsonData);
}

void BlockManager::Initiate() {
    _sandRendererRef = COMPONENTSMANAGER.GetComponentByID<InstanceRenderer>(_sandRendererRefID);
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(_cameraRefID);
    Component::Initiate();
}

void BlockManager::Init() {
    GenerateMap(0.5f,7);
    GenerateTopLayer(300,300);
    GenerateSphereVectors(31);
    UpdateBlocksVisibility();
    RefreshVisibleBlocks();
    UpdateRenderedChunks();
}

void BlockManager::UpdateInstanceRenderer() {
    std::vector<glm::mat4> instanceMatrix;

    // Iterate through _renderedChunks and add visible non-empty blocks to instanceMatrix
    for (int chunkIndex : _renderedChunks) {
        // Ensure chunkIndex is within bounds
        if (chunkIndex < 0 || chunkIndex >= _visibleBlocks.size()) {
            // Handle out-of-bounds error
            continue;
        }

        // Iterate through the blocks in the chunk
        for (BlockData* blockPtr : _visibleBlocks[chunkIndex]) {
            if (blockPtr->GetBlockType() != BlockType::EMPTY && blockPtr->IsVisible()) {
                // Add the block's transform matrix to instanceMatrix
                instanceMatrix.push_back(blockPtr->GetMatrix());
            }
        }
    }

    // Pass the instanceMatrix to _sandRendererRef
    if (_sandRendererRef) {
        _sandRendererRef->SetInstanceMatrix(instanceMatrix);
    }
}


void BlockManager::RefreshVisibleBlocks(int chunkIndex) {
    // Iterate over the block vector of the specified chunkIndex
    auto& blockVector = _visibleBlocks[chunkIndex];
    blockVector.erase(std::remove_if(blockVector.begin(), blockVector.end(),
        [](BlockData* blockPtr) {
            return blockPtr->GetBlockType() == BlockType::EMPTY || !blockPtr->IsVisible();
        }), blockVector.end());
}


void BlockManager::RefreshVisibleBlocks() {
    for (auto& blockVector : _visibleBlocks) {
        // Use erase-remove idiom to remove empty or non-visible blocks
        blockVector.erase(std::remove_if(blockVector.begin(), blockVector.end(),
            [](BlockData* blockPtr) {
                return blockPtr->GetBlockType() == BlockType::EMPTY || !blockPtr->IsVisible();
            }), blockVector.end());
    }
}


void BlockManager::UpdateBlocksVisibility() {
    // Clear existing visible blocks
    _visibleBlocks.clear();

    // Calculate the number of chunks in each dimension
    int chunkCountX = (_width + _chunkSize - 1) / _chunkSize;
    int chunkCountY = (_height + _chunkSize - 1) / _chunkSize;
    int chunkCountZ = (_depth + _chunkSize - 1) / _chunkSize;

    // Initialize _visibleBlocks with empty vectors for each chunk
    _visibleBlocks.resize(chunkCountX * chunkCountY * chunkCountZ);

    // Iterate through all blocks
    for (auto& blockData : _blocksData) {
        UpdateBlockVisibility(blockData);
    }

    //HideEdges();
}

void BlockManager::HideEdges()
{
    for (auto& blocks : _visibleBlocks) {
        for (BlockData* blockData : blocks) {
            if (blockData->GetPosID().y == 0) {
                blockData->SetVisible(false);
            }
            else if (blockData->GetPosID().y != _height - 1) {
                if (blockData->GetPosID().x == 0 || blockData->GetPosID().x == _width - 1) blockData->SetVisible(false);
                else if (blockData->GetPosID().z == 0 || blockData->GetPosID().z == _depth - 1) blockData->SetVisible(false);
            }
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

    blockData.SetSideCollisions(rightBlockEmpty, leftBlockEmpty, topBlockEmpty, bottomBlockEmpty, frontBlockEmpty, backBlockEmpty);

    // Set the visibility of the block
    SetVisibility(blockData, isVisible);
}

void BlockManager::UpdateRenderedChunks() {
    _renderedChunks.clear(); // Clear the list of rendered blocks


    for (const glm::ivec3& offset : _sphereVectors[_renderDistance]) {
        glm::ivec3 chunkPos = _playerChunk + offset;
        if(ChunkInBounds(chunkPos))
            _renderedChunks.push_back(GetChunkIndex(chunkPos));
    }

    UpdateInstanceRenderer();
}

void BlockManager::SetVisibility(BlockData& blockData, bool state) {
    // Set the visibility of the block
    blockData.SetVisible(state);

    if (state && blockData.GetBlockType() != BlockType::EMPTY) {
        // Add the blockData to _visibleBlocks
        _visibleBlocks[GetChunkIndex(blockData.GetChunkID(_chunkSize))].push_back(&blockData);
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

bool BlockManager::RayIntersectsBlock(float rayLength, int radius, float digPower) {
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
                DamageBlocks(roundedPoint,radius,digPower);
                return true; // Block hit, no need to check further
            }
        }
    }
    return false; // No block hit along the ray
}

void BlockManager::DamageBlocks(glm::ivec3 hitPos, int radius, float digPower)
{
    bool updateFlag = false;
    std::unordered_set<int> chunksToUpdate;

    for (const glm::ivec3& offset : _sphereVectors[radius]) {
        glm::ivec3 pos = hitPos + offset;
        if (InBounds(pos)) {
            int index = GetIndex(pos);
            if (_blocksData[index].GetBlockType() != BlockType::EMPTY) {
                if (_blocksData[index].DamageBlock(digPower)) {
                    _blocksData[index].SetBlockType(BlockType::EMPTY);
                    UpdateNeighbourVisibility(_blocksData[index]);
                    chunksToUpdate.insert(GetChunkIndex(_blocksData[index].GetChunkID(_chunkSize)));
                    updateFlag = true;
                }
            }
        }
    }

    if (updateFlag) {
        for (int chunkIndex : chunksToUpdate) {
            RefreshVisibleBlocks(chunkIndex);
        }
        UpdateRenderedChunks();
    }
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

void BlockManager::GenerateMap(float initialFillRatio, int numIterations) {
    // Initialize the map with a given initial fill ratio (percentage of cells initially filled)
    InitializeMap(initialFillRatio);

    // Iterate through the cellular automaton for a given number of iterations
    for (int i = 0; i < numIterations; ++i) {
        IterateCaveGeneration();
    }

    // Calculate the center of the top layer
    int centerX = _width / 2;
    int centerY = _height - 1; // assuming the top layer is the highest y value
    int centerZ = _depth / 2;

    // Define the mask dimensions
    glm::ivec3 maskDimensions(4, 6, 4);

    // Apply the mask using _entranceMask
    ApplyMask(glm::ivec3(centerX-2, centerY, centerZ-2), _entranceMask, maskDimensions);
}

void BlockManager::GenerateTopLayer(int sizeX, int sizeZ)
{
    std::vector<glm::mat4> instanceMatrix;
    // Iterate over the grid dimensions
    for (int x = 0; x < sizeX; x++) {
        for (int z = 0; z < sizeZ; z++) {

            // Calculate transform matrix for the current block
            glm::mat4 transformMatrix = Transform::CalculateTransformMatrix(glm::vec3(x, _height, z), glm::quat(), glm::vec3(1.0f));

            // Add the block to the vector
            instanceMatrix.push_back(transformMatrix);
        }
    }
    _topLayerRendererRef->SetInstanceMatrix(instanceMatrix);
}

void BlockManager::InitializeMap(float initialFillRatio) {
    // Initialize the random number generator
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    // Clear existing block data
    _blocksData.clear();

    // Iterate over the grid dimensions
    for (int y = 0; y < _height; y++) {
        for (int z = 0; z < _depth; z++) {
            for (int x = 0; x < _width; x++) {
                // Determine if the cell should initially be filled based on the fill ratio
                bool filled = dis(gen) < initialFillRatio;
                bool isEdgeBlock = IsEdgeBlock(x, y, z);

                if (isEdgeBlock)
                    filled = true;

                // Calculate transform matrix for the current block
                glm::mat4 transformMatrix = Transform::CalculateTransformMatrix(glm::vec3(x, y, z), glm::quat(), glm::vec3(1.0f));

                // Create BlockData object with appropriate HP based on block type
                BlockType type = filled ? BlockType::SAND : BlockType::EMPTY;
                float hp = 0.0f;
                if (type == BlockType::SAND) {
                    if (y < 100) {
                        hp = 7.0f;
                    }
                    else if (y < 200) {
                        hp = 3.0f;
                    }
                    else {
                        hp = 1.0f;
                    }
                }

                BlockData block(type, glm::ivec3(x, y, z), transformMatrix, hp, isEdgeBlock, 1.0f, shared_from_this());

                // Add the block to the vector
                _blocksData.push_back(block);
            }
        }
    }
}

void BlockManager::IterateCaveGeneration() {
    // Create a temporary vector to hold the new block data after iteration
    for (auto& blockData : _blocksData) {
        int filledNeighbors = 0;

        // Get the position of the current block
        glm::ivec3 posID = blockData.GetPosID();
        int x = posID.x;
        int y = posID.y;
        int z = posID.z;

        if (!IsEdgeBlock(x, y, z))
        {
            // Check adjacency for each direction and count filled neighbors
            filledNeighbors += (x - 1 < 0 || CheckAdjacency(x - 1, y, z));
            filledNeighbors += (x + 1 >= _width || CheckAdjacency(x + 1, y, z));
            filledNeighbors += (y + 1 >= _height || CheckAdjacency(x, y + 1, z));
            filledNeighbors += (y - 1 < 0 || CheckAdjacency(x, y - 1, z));
            filledNeighbors += (z + 1 >= _depth || CheckAdjacency(x, y, z + 1));
            filledNeighbors += (z - 1 < 0 || CheckAdjacency(x, y, z - 1));

            // Apply rules of cellular automaton to update the block state
            if (filledNeighbors >= 4) {
                ChangeType(blockData, BlockType::SAND);
            }
            else if (filledNeighbors <= 2) {
                ChangeType(blockData, BlockType::EMPTY);
            }
            else {
                float probability = 0.55f;
                if (static_cast<float>(rand()) / RAND_MAX < probability) {
                    ChangeType(blockData, BlockType::SAND);
                }
                else {
                    ChangeType(blockData, BlockType::EMPTY);
                }
            }
        }
    }
}

std::pair<glm::vec3,glm::vec3> BlockManager::CheckEntityCollision(glm::vec3 entityPos, glm::vec3 movementVector, float entityWidth, float entityHeight) {
    CheckEntityChunk(entityPos);

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

void BlockManager::CheckEntityChunk(glm::vec3 entityPos) {
    glm::ivec3 gridPosition = glm::round(entityPos);

    // Initialize newEntityChunk
    glm::ivec3 newEntityChunk(gridPosition.x / _chunkSize, gridPosition.y / _chunkSize, gridPosition.z / _chunkSize);

    if (newEntityChunk != _playerChunk) {
        _playerChunk = newEntityChunk;
        UpdateRenderedChunks();
    }
}

void BlockManager::ChangeType(BlockData& blockData, BlockType type)
{
    blockData.SetBlockType(type);
    switch (type)
    {
        case BlockType::EMPTY:
            break;
        case BlockType::SAND:
                if (blockData.GetPosID().y < 100) {
                    blockData.SetHP(7.0f);
                }
                else if (blockData.GetPosID().y < 200) {
                    blockData.SetHP(3.0f);
                }
                else {
                    blockData.SetHP(1.0f);
                }
            break;
        default:
            break;
    }
}

void BlockManager::ApplyMask(glm::ivec3 startPos, int* maskArray, glm::ivec3 maskDimensions) {
    int maskSizeX = maskDimensions.x;
    int maskSizeY = maskDimensions.y;
    int maskSizeZ = maskDimensions.z;

    // Iterate through y
    for (int y = 0; y < maskSizeY; ++y) {
        // Iterate through x
        for (int x = 0; x < maskSizeX; ++x) {
            // Iterate through z
            for (int z = 0; z < maskSizeZ; ++z) {
                // Calculate the actual position in the mask array
                int posX = startPos.x + x;
                int posY = startPos.y - y;
                int posZ = startPos.z + z;

                // Access maskArray element using linear index calculation
                int index = z + maskSizeZ * (x + maskSizeX * y);
                int maskValue = maskArray[index];

                if (InBounds(posX, posY, posZ)) {
                    int blockIndex = GetIndex(posX, posY, posZ);

                    // Cast maskValue to BlockType and pass to ChangeType
                    ChangeType(_blocksData[blockIndex], static_cast<BlockType>(maskValue));
                }
            }
        }
    }
}

int BlockManager::GetIndex(glm::ivec3 point) {
    return point.x + (_width * _depth * point.y) + point.z * _width;
}

int BlockManager::GetIndex(int x, int y, int z) {
    return x + (_width * _depth * y) + z * _width;
}

int BlockManager::GetChunkIndex(glm::ivec3 chunk) {
    return chunk.x + ((_width / _chunkSize) * (_depth / _chunkSize) * chunk.y) + chunk.z * (_width / _chunkSize);
}

int BlockManager::GetChunkIndex(int x, int y, int z) {
    return x + ((_width / _chunkSize) * (_depth / _chunkSize) * y) + z * (_width / _chunkSize);
}

bool BlockManager::CheckAdjacency(int x, int y, int z){
    return _blocksData[GetIndex(x, y, z)].GetBlockType() != BlockType::EMPTY;
}

bool BlockManager::InBounds(glm::ivec3 position) {
    return position.x >= 0 && position.y >= 0 && position.z >= 0 &&
        position.x < _width && position.y < _height && position.z < _depth;
}

bool BlockManager::InBounds(int x, int y, int z) {
    return x >= 0 && y >= 0 && z >= 0 &&
        x < _width && y < _height && z < _depth;
}

bool BlockManager::ChunkInBounds(glm::ivec3 position) {
    int numChunksX = (_width + _chunkSize - 1) / _chunkSize;
    int numChunksY = (_height + _chunkSize - 1) / _chunkSize;
    int numChunksZ = (_depth + _chunkSize - 1) / _chunkSize;

    return position.x >= 0 && position.y >= 0 && position.z >= 0 &&
        position.x < numChunksX && position.y < numChunksY && position.z < numChunksZ;
}

bool BlockManager::IsEdgeBlock(int x, int y, int z)
{
    return (x == 0 || x == _width - 1 || y == 0 || y == _height - 1 || z == 0 || z == _depth - 1);
}

void BlockManager::addToInspector(ImguiMain *imguiMain) {
    if (ImGui::TreeNode("BlockManager")) {
        // Gui related things go there
        ImGui::Text("Test1:");
        ImGui::Text("Test2:");

        ImGui::TreePop();
    }
}
