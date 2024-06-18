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

    if (_plasticRendererRef) {
        data["_plasticRendererRef"] = _plasticRendererRef->_id;
    }

    if (_metalRendererRef) {
        data["_metalRendererRef"] = _metalRendererRef->_id;
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
        _topLayerRendererRefID = jsonData["topLayerRendererRefID"].get<int>();
    }

    if (jsonData.contains("plasticRendererRefID")) {
        _plasticRendererRefID = jsonData["plasticRendererRefID"].get<int>();
    }

    if (jsonData.contains("metalRendererRefID")) {
        _metalRendererRefID = jsonData["metalRendererRefID"].get<int>();
    }

    if (jsonData.contains("cameraRefID")) {
        _cameraRefID = jsonData["cameraRefID"].get<int>();
    }

    Component::Deserialize(jsonData);
}

void BlockManager::Initiate() {
    _sandRendererRef = COMPONENTSMANAGER.GetComponentByID<InstanceRenderer>(_sandRendererRefID);
    _topLayerRendererRef = COMPONENTSMANAGER.GetComponentByID<InstanceRenderer>(_topLayerRendererRefID);
    _plasticRendererRef = COMPONENTSMANAGER.GetComponentByID<InstanceRenderer>(_plasticRendererRefID);
    _metalRendererRef = COMPONENTSMANAGER.GetComponentByID<InstanceRenderer>(_metalRendererRefID);
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(_cameraRefID);
    Component::Initiate();
}

void BlockManager::Init() {
    InitLayerStats();
    GenerateSphereVectors(31);
    GenerateMap(0.5f,7);
    GenerateResources();
    ApplyMasks();
    GenerateTopLayer(glm::ivec2(50, 50), glm::ivec2(500, 500), glm::ivec2(50, 50));
    UpdateBlocksVisibility();
    RefreshVisibleBlocks();
    UpdateRenderedChunks();
}

void BlockManager::Reset()
{
    GenerateMap(0.5f, 7);
    GenerateResources();
    ApplyMasks();
    GenerateTopLayer(glm::ivec2(50, 50), glm::ivec2(500, 500), glm::ivec2(50, 50));
    UpdateBlocksVisibility();
    RefreshVisibleBlocks();
    UpdateRenderedChunks();
}

void BlockManager::UpdateInstanceRenderer() {
    std::vector<glm::vec3> instancedSandPosition;
    std::vector<glm::vec3> instancedPlasticPosition;
    std::vector<glm::vec3> instancedMetalPosition;

    // Iterate through _renderedChunks and add visible non-empty blocks to instanceMatrix
    for (int chunkIndex : _renderedChunks) {
        // Ensure chunkIndex is within bounds
        if (chunkIndex < 0 || chunkIndex >= _visibleBlocks.size()) {
            // Handle out-of-bounds error
            continue;
        }

        // Iterate through the blocks in the chunk
        for (BlockData* blockPtr : _visibleBlocks[chunkIndex]) {
            switch (blockPtr->GetBlockType()) {
                case BlockType::DIRT:
                    instancedSandPosition.push_back(blockPtr->GetPosID());
                    break;
                case BlockType::PLASTIC:
                    instancedPlasticPosition.push_back(blockPtr->GetPosID());
                    break;
                case BlockType::PLASTIC_BIG:
                    instancedMetalPosition.push_back(blockPtr->GetPosID());
            }
        }
    }

    // Pass the instanceMatrix to _sandRendererRef
    if (_sandRendererRef) {
        _sandRendererRef->RefreshPositionBuffer(instancedSandPosition);
    }
    if (_plasticRendererRef) {
        _plasticRendererRef->RefreshPositionBuffer(instancedPlasticPosition);
    }
    if (_metalRendererRef) {
        _metalRendererRef->RefreshPositionBuffer(instancedMetalPosition);
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

    HideEdges();
    UpdateVisibilityNearResources();
}

void BlockManager::HideEdges()
{
    for (BlockData& blockData : _blocksData) {
        if (IsEdgeBlock(blockData)) {
            blockData.SetVisible(false);
        }
    }
}

void BlockManager::UpdateNeighbourVisibility(BlockData& blockData)
{
    glm::ivec3 posID = blockData.GetPosID();
    int x = posID.x;
    int y = posID.y;
    int z = posID.z;

    if (x - 1 >= 0 && x - 1 < _width) UpdateBlockVisibility(_blocksData[GetIndex(x - 1, y, z)]); // Left
    if (x + 1 >= 0 && x + 1 < _width) UpdateBlockVisibility(_blocksData[GetIndex(x + 1, y, z)]); // Right
    if (y + 1 >= 0 && y + 1 < _height) UpdateBlockVisibility(_blocksData[GetIndex(x, y + 1, z)]); // Top
    if (y - 1 >= 0 && y - 1 < _height) UpdateBlockVisibility(_blocksData[GetIndex(x, y - 1, z)]); // Bottom
    if (z + 1 >= 0 && z + 1 < _depth) UpdateBlockVisibility(_blocksData[GetIndex(x, y, z + 1)]); // Front
    if (z - 1 >= 0 && z - 1 < _depth) UpdateBlockVisibility(_blocksData[GetIndex(x, y, z - 1)]); // Back
}

void BlockManager::UpdateNeighbourVisibility(BlockData& blockData,bool state)
{
    glm::ivec3 posID = blockData.GetPosID();
    int x = posID.x;
    int y = posID.y;
    int z = posID.z;

    if (x - 1 >= 0 && x - 1 < _width) SetVisibility(_blocksData[GetIndex(x - 1, y, z)], state); // Left
    if (x + 1 >= 0 && x + 1 < _width) SetVisibility(_blocksData[GetIndex(x + 1, y, z)], state); // Right
    if (y + 1 >= 0 && y + 1 < _height) SetVisibility(_blocksData[GetIndex(x, y + 1, z)], state); // Top
    if (y - 1 >= 0 && y - 1 < _height) SetVisibility(_blocksData[GetIndex(x, y - 1, z)], state); // Bottom
    if (z + 1 >= 0 && z + 1 < _depth) SetVisibility(_blocksData[GetIndex(x, y, z + 1)], state); // Front
    if (z - 1 >= 0 && z - 1 < _depth) SetVisibility(_blocksData[GetIndex(x, y, z - 1)], state); // Back
}

void BlockManager::UpdateBlockVisibility(BlockData& blockData)
{
    // Get the position of the current block
    glm::ivec3 posID = blockData.GetPosID();
    int x = posID.x;
    int y = posID.y;
    int z = posID.z;

    // Check adjacent blocks for emptiness
    bool leftBlockSolid = x - 1 >= 0 && CheckAdjacency(x - 1, y, z);
    bool rightBlockSolid = x + 1 < _width && CheckAdjacency(x + 1, y, z);
    bool bottomBlockSolid = y - 1 >= 0 && CheckAdjacency(x, y - 1, z);
    bool topBlockSolid = y + 1 < _height && CheckAdjacency(x, y + 1, z);
    bool backBlockSolid = z - 1 >= 0 && CheckAdjacency(x, y, z - 1);
    bool frontBlockSolid = z + 1 < _depth && CheckAdjacency(x, y, z + 1);

    // Check if the block will be visible
    bool isVisible = !leftBlockSolid || !rightBlockSolid || !topBlockSolid || !bottomBlockSolid || !frontBlockSolid || !backBlockSolid;

    blockData.SetCollSides(!leftBlockSolid, !rightBlockSolid, !bottomBlockSolid, !topBlockSolid, !backBlockSolid, !frontBlockSolid);

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
    if (state && blockData.IsSolid() && !IsEdgeBlock(blockData)) {
        blockData.SetVisible(state);

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
            if (_blocksData[index].IsSolid()) {
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
            if (_blocksData[index].IsSolid()) {
                if (_blocksData[index].DamageBlock(digPower)) {
                    chunksToUpdate.insert(DestroyBlock(_blocksData[index]));
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

void BlockManager::GenerateMap(float initialFillRatio, int numIterations) {
    // Initialize the map with a given initial fill ratio (percentage of cells initially filled)
    InitializeMap(initialFillRatio);

    // Iterate through the cellular automaton for a given number of iterations
    for (int i = 0; i < numIterations; ++i) {
        IterateCaveGeneration();
    }

    GenerateTunnels();
}

void BlockManager::GenerateTopLayer(glm::ivec2 center, glm::ivec2 dimensions, glm::ivec2 deadzone) {
    std::vector<glm::vec3> instancePositions;
    _topLayerPositions.clear();

    float frequency = 0.025f; // Frequency of the noise (controls the detail level)
    float amplitude = 10.0f; // Amplitude of the noise (controls the height variation)
    float smoothEnd = 0.35f;

    int startX = center.x - dimensions.x / 2;
    int startZ = center.y - dimensions.y / 2;

    // Calculate the maximum distance from the edge of the deadzone to the point where full noise height is applied
    int maxDistanceX = dimensions.x / 2 - deadzone.x;
    int maxDistanceZ = dimensions.y / 2 - deadzone.y;

    // Seed for randomness
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1000.0f);

    // Generate random offsets for the Perlin noise
    float randomOffsetX = dis(gen);
    float randomOffsetZ = dis(gen);

    // Iterate over the grid dimensions
    for (int x = startX; x < startX + dimensions.x; x++) {
        for (int z = startZ; z < startZ + dimensions.y; z++) {
            // Calculate distance from the center of the deadzone
            int distX = abs(x - center.x) - deadzone.x;
            int distZ = abs(z - center.y) - deadzone.y;

            // If inside the deadzone, skip the block
            if (distX < 0 && distZ < 0) {
                continue;
            }

            // Generate a height value using Perlin noise with random offsets
            float height = glm::perlin(glm::vec2((x + randomOffsetX) * frequency, (z + randomOffsetZ) * frequency)) * amplitude;

            float smoothingFactor = glm::clamp(float(distX + distZ) / float((maxDistanceX + maxDistanceZ) / (1 / smoothEnd)), 0.0f, 1.0f);

            height *= smoothingFactor;

            // Add the block to the vector
            instancePositions.push_back(glm::vec3(x, (_height - 1) + height, z));
        }
    }
    _topLayerPositions = instancePositions;

    // Iterate over _blocksData and add edge blocks to instanceMatrix
    for (BlockData& blockData : _blocksData) {
        if (IsEdgeBlock(blockData) && blockData.IsSolid()) {
            instancePositions.push_back(blockData.GetPosID());
        }
    }

    _topLayerRendererRef->RefreshPositionBuffer(instancePositions);
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

                // Create BlockData object with appropriate HP based on block type
                BlockType type = filled ? BlockType::DIRT : BlockType::EMPTY;
                float hp = 0.0f;
                if (type == BlockType::DIRT) {
                    hp = GetBlockHP(y);
                }

                BlockData block(type, glm::ivec3(x, y, z), hp, isEdgeBlock, 1.0f, shared_from_this());

                // Add the block to the vector
                _blocksData.push_back(block);
            }
        }
    }
}

int BlockManager::GetMaterialReward(BlockData& blockData)
{
    if (blockData.GetPosID().y < 100) {
        return _layerStats[2].second;
    }
    else if (blockData.GetPosID().y < 200) {
        return _layerStats[1].second;
    }
    else {
        return _layerStats[0].second;
    }
}

float BlockManager::GetBlockHP(BlockData& blockData)
{
    if (blockData.GetPosID().y < 100) {
        return _layerStats[2].first;
    }
    else if (blockData.GetPosID().y < 200) {
        return _layerStats[1].first;
    }
    else {
        return _layerStats[0].first;
    }
}

float BlockManager::GetBlockHP(float y)
{
    if (y < 100) {
        return _layerStats[2].first;
    }
    else if (y < 200) {
        return _layerStats[1].first;
    }
    else {
        return _layerStats[0].first;
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
                ChangeType(blockData, BlockType::DIRT);
            }
            else if (filledNeighbors <= 2) {
                ChangeType(blockData, BlockType::EMPTY);
            }
            else {
                float probability = 0.55f;
                if (static_cast<float>(rand()) / RAND_MAX < probability) {
                    ChangeType(blockData, BlockType::DIRT);
                }
                else {
                    ChangeType(blockData, BlockType::EMPTY);
                }
            }
        }
    }
}

std::vector<CollisionInfo> BlockManager::CalculateCollisionInfo(glm::vec3 entityPos, glm::vec3 movementVector, float halfWidth, float entityHeight) {
    std::vector<CollisionInfo> collisionInfoList;
    const float tolerance = 0.0001f;
    // Calculate the number of layers based on the entity's height
    int numLayers = static_cast<int>(glm::ceil(entityHeight));

    // Generate corners dynamically
    std::vector<glm::vec3> corners;
    for (int i = 0; i <= numLayers; ++i) {
        float yOffset = (i == numLayers) ? entityHeight : static_cast<float>(i);
        corners.push_back(entityPos + glm::vec3(-halfWidth, yOffset, -halfWidth));
        corners.push_back(entityPos + glm::vec3(halfWidth, yOffset, -halfWidth));
        corners.push_back(entityPos + glm::vec3(-halfWidth, yOffset, halfWidth));
        corners.push_back(entityPos + glm::vec3(halfWidth, yOffset, halfWidth));
    }

    // Iterate through the corners and check collision with the nearest blocks
    for (size_t i = 0; i < corners.size(); ++i) {
        // Round the position of the corner to get the ID of the nearest block
        glm::ivec3 roundedPosXZ = glm::round(glm::vec3(corners[i].x + movementVector.x, corners[i].y, corners[i].z + movementVector.z));
        glm::ivec3 roundedPosY = glm::round(glm::vec3(corners[i].x, corners[i].y + movementVector.y, corners[i].z));

        // Initialize CollisionInfo for the current corner
        CollisionInfo info;

        // Check if the rounded position is within bounds for X-axis
        if (InBounds(roundedPosXZ)) {
            int index = GetIndex(roundedPosXZ);

            // Check if the block at the index is not empty
            if (_blocksData[index].IsSolid()) {
                // Calculate the AABB extents of the block
                glm::vec3 blockMin = glm::vec3(roundedPosXZ) - glm::vec3(0.5f);
                glm::vec3 blockMax = blockMin + glm::vec3(1.0f);

                // Calculate the direction for x-axis based on the entity's position relative to the block
                float entityPosX = entityPos.x + movementVector.x;

                // X-axis collision check
                if (entityPosX < roundedPosXZ.x) {
                    if (_blocksData[index].GetCollSide(0) && entityPosX + halfWidth > blockMin.x) {
                        // Determine the separation vector in the x-axis for the negative x side
                        info.separationVector.x = (blockMin.x - (entityPosX + halfWidth)) - tolerance;
                        info.isColliding = true;
                    }
                }
                else {
                    if (_blocksData[index].GetCollSide(1) && entityPosX - halfWidth < blockMax.x) {
                        // Determine the separation vector in the x-axis for the positive x side
                        info.separationVector.x = (blockMax.x - (entityPosX - halfWidth)) + tolerance;
                        info.isColliding = true;
                    }
                }

                // Calculate the direction for z-axis based on the entity's position relative to the block
                float entityPosZ = entityPos.z + movementVector.z;

                // Z-axis collision check
                if (entityPosZ < roundedPosXZ.z) {
                    if (_blocksData[index].GetCollSide(4) && entityPosZ + halfWidth > blockMin.z) {
                        // Determine the separation vector in the z-axis for the negative z side
                        info.separationVector.z = (blockMin.z - (entityPosZ + halfWidth)) - tolerance;
                        info.isColliding = true;
                    }
                }
                else {
                    if (_blocksData[index].GetCollSide(5) && entityPosZ - halfWidth < blockMax.z) {
                        // Determine the separation vector in the z-axis for the positive z side
                        info.separationVector.z = (blockMax.z - (entityPosZ - halfWidth)) + tolerance;
                        info.isColliding = true;
                    }
                }
            }
        }


        // Check if the rounded position is within bounds for Y-axis
        if (InBounds(roundedPosY)) {
            int index = GetIndex(roundedPosY);

            // Check if the block at the index is not empty
            if (_blocksData[index].IsSolid()) {
                // Calculate the AABB extents of the block
                glm::vec3 blockMin = glm::vec3(roundedPosY) - glm::vec3(0.5f);
                glm::vec3 blockMax = blockMin + glm::vec3(1.0f);

                float entityPosY = entityPos.y + movementVector.y;
                if (i < 4) {
                    if (_blocksData[index].GetCollSide(3) && entityPosY < blockMax.y) {
                        info.separationVector.y = blockMax.y - entityPosY + tolerance;
                        info.isColliding = true;
                    }
                }
                else if (i >= (corners.size() - 4)) {
                    if (_blocksData[index].GetCollSide(2) && entityPosY + entityHeight > blockMin.y) {
                        info.separationVector.y = blockMin.y - (entityPosY + entityHeight) - tolerance;
                        info.isColliding = true;
                    }
                }
            }
        }

        if(info.isColliding)
            collisionInfoList.push_back(info);
    }

    // Return the list of collision information for each corner
    return collisionInfoList;
}

std::pair<glm::vec3, glm::vec3> BlockManager::CheckEntityCollision(glm::vec3 entityPos, glm::vec3 movementVector, float entityWidth, float entityHeight) {
    CheckEntityChunk(entityPos);

    // Calculate the half extents of the entity's AABB
    float halfWidth = entityWidth / 2.0f;

    // Calculate collision information for each corner of the entity's AABB
    std::vector<CollisionInfo> collisionInfoList = CalculateCollisionInfo(entityPos, movementVector, halfWidth, entityHeight);

    float smallestX = std::numeric_limits<float>::max();
    float smallestY = std::numeric_limits<float>::max();
    float smallestZ = std::numeric_limits<float>::max();

    for (const CollisionInfo& collisionInfo : collisionInfoList) {
        // Update separationVector based on the smallest non-zero component
        if (std::abs(collisionInfo.separationVector.x) < std::abs(collisionInfo.separationVector.z)) {
            if (std::abs(collisionInfo.separationVector.x) < std::abs(smallestX))
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

    // Debug: Print the movementVector and separationVector
   //std::cout << "Movement Vector: " << movementVector.x << ", " << movementVector.y << ", " << movementVector.z << std::endl;
   //std::cout << "Separation Vector: " << separationVector.x << ", " << separationVector.y << ", " << separationVector.z << std::endl;

    movementVector += separationVector;

    // Return the adjusted movementVector and the separationVector
    return std::make_pair(movementVector, separationVector);
}


std::tuple<bool, BlockData*, glm::vec3> BlockManager::CheckSimpleEntityCollision(glm::vec3 entityPos) {
    glm::ivec3 roundedPos = glm::round(entityPos);
    glm::vec3 separationVector = glm::vec3(0.0f);

    if (InBounds(roundedPos)) {
        int index = GetIndex(roundedPos);

        // Check if the block at the index is not empty
        if (_blocksData[index].IsSolid()) {
            // Calculate the AABB extents of the block
            glm::vec3 blockMin = glm::vec3(roundedPos) - glm::vec3(0.5f);
            glm::vec3 blockMax = blockMin + glm::vec3(1.0f);

            // X-axis separation
            if (entityPos.x > blockMin.x && entityPos.x < blockMax.x) {
                float directionX = glm::sign(entityPos.x - roundedPos.x);
                separationVector.x = (std::abs(entityPos.x - roundedPos.x) - 0.5f) * -directionX;
            }

            // Y-axis separation
            if (entityPos.y < blockMax.y) {
                separationVector.y = std::abs(entityPos.y - blockMax.y);
            }
            else if (entityPos.y > blockMin.y) {
                separationVector.y = -std::abs(entityPos.y - blockMin.y);
            }

            // Z-axis separation
            if (entityPos.z > blockMin.z && entityPos.z < blockMax.z) {
                float directionZ = glm::sign(entityPos.z - roundedPos.z);
                separationVector.z = (std::abs(entityPos.z - roundedPos.z) - 0.5f) * -directionZ;
            }

            // Find the smallest component of the separation vector
            float smallestComponent = std::min(std::min(std::abs(separationVector.x), std::abs(separationVector.y)), std::abs(separationVector.z));

            // Set other components to zero except the smallest one
            if (std::abs(separationVector.x) == smallestComponent) {
                separationVector.y = 0.0f;
                separationVector.z = 0.0f;
            }
            else if (std::abs(separationVector.y) == smallestComponent) {
                separationVector.x = 0.0f;
                separationVector.z = 0.0f;
            }
            else {
                separationVector.x = 0.0f;
                separationVector.y = 0.0f;
            }

            return std::make_tuple(true, &_blocksData[index], separationVector);
        }
    }

    // If no collision, return false and nullptr
    return std::make_tuple(false, nullptr, glm::vec3(0.0f));
}


float BlockManager::GetCaveFloor(glm::vec3 entityPos, int maxDistance) {
    glm::ivec3 roundedPos = glm::round(entityPos);
    int startY = roundedPos.y;
    int distance = 0;

    do {
        if (InBounds(roundedPos))
        {
            int index = GetIndex(roundedPos);

            // Check if the block at the index is solid
            if (_blocksData[index].IsSolid()) {
                return roundedPos.y + 1.0f;
            }
        }

        // Move down to the next block
        roundedPos.y -= 1;
        distance++;
    } while ( distance <= maxDistance);

    // Return entityPos.y - maxDistance if no solid block is found within the maxDistance
    return glm::round(entityPos.y) - maxDistance + 1.0f;
}

float BlockManager::GetTopLayerFloor(glm::vec3 entityPos)
{
    float tolerance = 1.0f;

    for (const auto& pos : _topLayerPositions) {
        if (std::abs(pos.x - entityPos.x) < tolerance && std::abs(pos.z - entityPos.z) < tolerance) {
            return pos.y;
        }
    }
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
        case BlockType::PLASTIC_BIG:
        case BlockType::PLASTIC:
        case BlockType::DIRT:
            blockData.SetHP(GetBlockHP(blockData));
            break;
        default:
            break;
    }
}

void BlockManager::ApplyMask(glm::ivec3 startPos, char* maskArray, glm::ivec3 maskDimensions) {
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

void BlockManager::GenerateResources()
{
    std::random_device rd;  
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    // Define the probability of a block being changed to PLASTIC_BIG or PLASTIC
    const double plasticBigProbability = .25;   // 50% chance of being PLASTIC_BIG
    const double plasticProbability = .75; // 50% chance of being PLASTIC

    // Generate Poisson disk distribution points
    std::vector<glm::vec3> poissonPoints = GeneratePoissonDiskPoints();
    //std::cout <<"Materials: "<< poissonPoints.size() << std::endl;
    // Iterate through the Poisson disk points
    for (const auto& point : poissonPoints)
    {
        glm::ivec3 position = glm::round(point);

        // Check if the position is within bounds
        while (InBounds(position))
        {
            int index = GetIndex(position);

            // Check if the block below is not empty
            glm::ivec3 belowPosition = position + glm::ivec3(0, -1, 0);
            if (InBounds(belowPosition,1) && _blocksData[GetIndex(belowPosition)].IsSolid())
            {
                // Generate a random number between 0 and 1
                double randomValue = dis(gen);

                // Check if the block should be changed to METAL
                if (randomValue < plasticBigProbability)
                {
                    ChangeType(_blocksData[index], BlockType::PLASTIC_BIG);
                    break;
                }
                // Check if the block should be changed to PLASTIC
                else
                {
                    ChangeType(_blocksData[index], BlockType::PLASTIC);
                    break;
                }
            }

            // Move one block down
            position += glm::ivec3(0, -1, 0);
        }
    }
}

std::vector<glm::vec3> BlockManager::GeneratePoissonDiskPoints()
{
    std::vector<glm::vec3> points;
    // Parameters for Poisson disk sampling
    float minDist = 7.0f;  // Minimum distance between points
    int k = 10;             // Number of attempts

    // Define the bounds for sampling
    glm::vec3 sampleRegionSize = glm::vec3(_width, _height, _depth);
    std::vector<glm::vec3> spawnPoints;
    spawnPoints.push_back(sampleRegionSize / 2.0f);

    // Random number generator setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    while (!spawnPoints.empty())
    {
        int spawnIndex = static_cast<int>(dis(gen) * spawnPoints.size());
        glm::vec3 spawnCenter = spawnPoints[spawnIndex];
        bool candidateAccepted = false;

        for (int i = 0; i < k; ++i)
        {
            float angle = dis(gen) * 2.0f * glm::pi<float>();
            glm::vec3 dir = glm::vec3(glm::cos(angle), glm::sin(angle), glm::tan(angle));
            glm::vec3 candidate = spawnCenter + dir * minDist * (1.0f + dis(gen));

            if (InBounds(glm::round(candidate), 1) && !IsPointTooClose(points, candidate, minDist))
            {
                points.push_back(candidate);
                spawnPoints.push_back(candidate);
                candidateAccepted = true;
                break;
            }
        }

        if (!candidateAccepted)
        {
            spawnPoints.erase(spawnPoints.begin() + spawnIndex);
        }
    }

    return points;
}

void BlockManager::UpdateVisibilityNearResources()
{
    for (auto& blockData : _blocksData) {
        if(blockData.IsMaterial())
            UpdateNeighbourVisibility(blockData, true);
    }
}

int BlockManager::DestroyBlock(BlockData& blockData)
{
    glm::vec3 _posID;
    switch (blockData.GetBlockType())
    {
        case BlockType::PLASTIC:
            GAMEMANAGER.AddPlastic(GetMaterialReward(blockData));
            _posID = blockData.GetPosID();
            NODESMANAGER.getNodeByName(to_string(int(_posID.x)) + to_string(int(_posID.y)) + to_string(int(_posID.z)))->GetComponent<ParticleGenerator>()->toDelete = true;
            break;
        case BlockType::PLASTIC_BIG:
            GAMEMANAGER.AddPlastic(GetMaterialReward(blockData)*3);
            _posID = blockData.GetPosID();
            NODESMANAGER.getNodeByName(to_string(int(_posID.x)) + to_string(int(_posID.y)) + to_string(int(_posID.z)))->GetComponent<ParticleGenerator>()->toDelete = true;
            break;
        default:
            break;
    }
    blockData.SetBlockType(BlockType::EMPTY);
    UpdateNeighbourVisibility(blockData);
    blockData.UnstuckGlowsticks();
    return GetChunkIndex(blockData.GetChunkID(_chunkSize));
}

void BlockManager::ApplyMasks()
{
    // Calculate the center of the top layer
    int centerX = _width / 2;
    int centerY = _height - 1; // assuming the top layer is the highest y value
    int centerZ = _depth / 2;

    // Apply the mask using _entranceMask
    ApplyMask(glm::ivec3(centerX - 3, centerY, centerZ - 3), _entranceMask, _entranceMaskDimensions);

    ApplyMask(glm::ivec3(centerX - 11, centerY - 5, centerZ - 12), _tutorialCaveMask, _tutorialCaveMaskDimensions);
}

void BlockManager::GenerateTunnel(std::vector<glm::ivec3> points, int size)
{
    // Control points for the curve
    glm::ivec3 start = points.front();
    glm::ivec3 end = points.back();
    glm::ivec3 control = (points.size() > 2) ? points[1] : glm::ivec3((start.x + end.x) / 2, (start.y + end.y) / 2, (start.z + end.z) / 2 + 20);

    int numSteps = 100;

    std::vector<glm::ivec3> pathPoints;
    for (int i = 0; i <= numSteps; ++i) {
        float t = static_cast<float>(i) / numSteps;
        pathPoints.push_back(QuadraticBezier(start, control, end, t));
    }

    for (const glm::ivec3& currentPos : pathPoints) {
        for (const glm::ivec3& offset : _sphereVectors[size]) {
            glm::ivec3 pos = currentPos + offset;
            if (InBounds(pos) && !IsEdgeBlock(pos)) {
                int index = GetIndex(pos);
                ChangeType(_blocksData[index], BlockType::EMPTY);
            }
        }
    }
}

void BlockManager::GenerateTunnels() {
    // Random number generator setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    glm::ivec3 startLayer1 = GetRandomSideBlock(glm::ivec2(200, 250));
    //std::cout << "StartPoint for tunnel 1: " << startLayer1.x << ", " << startLayer1.y << ", " << startLayer1.z << std::endl;

    glm::ivec3 endLayer2 = GetRandomSideBlock(glm::ivec2(150, 250), startLayer1);
    //std::cout << "EndPoint for tunnel 1: " << endLayer2.x << ", " << endLayer2.y << ", " << endLayer2.z << std::endl;

    // Generate intermediate points within the map for the first tunnel
    std::vector<glm::ivec3> pointsTunnel1;
    pointsTunnel1.push_back(startLayer1);
    int numPointsTunnel1 = 1 + static_cast<int>(dis(gen) * 2); // Random number of intermediate points (1 to 2)
    for (int i = 0; i < numPointsTunnel1; ++i) {
        int interX = static_cast<int>(dis(gen) * _width);
        int interZ = static_cast<int>(dis(gen) * (_depth - 1));
        int interY = startLayer1.y + 20 + static_cast<int>(dis(gen) * (endLayer2.y - startLayer1.y - 40));
        glm::ivec3 point = glm::ivec3(interX, interY, interZ);
        pointsTunnel1.push_back(point);
        //std::cout << "Point for tunnel 1: " << point.x << ", " << point.y << ", " << point.z << std::endl;
    }
    pointsTunnel1.push_back(endLayer2);

    // Generate the first tunnel
    GenerateTunnel(pointsTunnel1, 6);



    glm::ivec3 startLayer2 = GetRandomSideBlock(glm::ivec2(100, 150));
    //std::cout << "StartPoint for tunnel 2: " << startLayer2.x << ", " << startLayer2.y << ", " << startLayer2.z << std::endl;

    glm::ivec3 endLayer3 = GetRandomSideBlock(glm::ivec2(50, 150), startLayer2);
    //std::cout << "EndPoint for tunnel 2: " << endLayer3.x << ", " << endLayer3.y << ", " << endLayer3.z << std::endl;

    // Generate intermediate points within the map for the second tunnel
    std::vector<glm::ivec3> pointsTunnel2;
    pointsTunnel2.push_back(startLayer2);
    int numPointsTunnel2 = 1 + static_cast<int>(dis(gen) * 2); // Random number of intermediate points (1 to 3)
    for (int i = 0; i < numPointsTunnel2; ++i) {
        int interX = static_cast<int>(dis(gen) * _width);
        int interZ = static_cast<int>(dis(gen) * (_depth - 1));
        int interY = startLayer2.y + 20 + static_cast<int>(dis(gen) * (endLayer3.y - startLayer2.y - 40));
        glm::ivec3 point = glm::ivec3(interX, interY, interZ);
        pointsTunnel2.push_back(point);
        //std::cout << "Point for tunnel 2: " << point.x << ", " << point.y << ", " << point.z << std::endl;
    }
    pointsTunnel2.push_back(endLayer3);

    // Generate the second tunnel
    GenerateTunnel(pointsTunnel2, 8);
}

void BlockManager::InitLayerStats() {
    // Clear existing layer stats if any
    _layerStats.clear();

    // Define the pairs of (float, int) values
    _layerStats.push_back({ 1.0f, 1 }); // HP, Mat reward
    _layerStats.push_back({ 3.0f, 3 }); // HP, Mat reward
    _layerStats.push_back({ 7.0f, 7 }); // HP, Mat reward
}


glm::ivec3 BlockManager::QuadraticBezier(const glm::ivec3& p0, const glm::ivec3& p1, const glm::ivec3& p2, float t) {
    float u = 1.0f - t;
    float tt = t * t;
    float uu = u * u;

    glm::vec3 p = uu * glm::vec3(p0); // (1-t)^2 * P0
    p += 2 * u * t * glm::vec3(p1);   // 2(1-t)t * P1
    p += tt * glm::vec3(p2);          // t^2 * P2

    return glm::ivec3(glm::round(p));
}

glm::ivec3 BlockManager::GetRandomSideBlock(glm::ivec2 yRange)
{
    // Random number generator setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    int x = 0;
    int z = 0;

    if (dis(gen) > 0.5f) {
        x = (dis(gen) > 0.5f) ? (_width - 1) : 0;
        z = static_cast<int>(dis(gen) * (_depth - 1));
    }
    else {
        x = static_cast<int>(dis(gen) * (_width - 1));
        z = (dis(gen) > 0.5f) ? (_depth - 1) : 0;
    }

    return glm::ivec3(x, yRange.x + static_cast<int>(dis(gen) * (yRange.y - yRange.x)), z);
}

glm::ivec3 BlockManager::GetRandomSideBlock(glm::ivec2 yRange, glm::ivec3 exclude)
{
    // Random number generator setup
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> dis(0.0f, 1.0f);

    int x = 0;
    int z = 0;

    if (dis(gen) > 0.5f && (exclude.x != (_width - 1) || exclude.x != 0)) {
        x = (dis(gen) > 0.5f) ? (_width - 1) : 0;
        z = static_cast<int>(dis(gen) * (_depth - 1));
    }
    else {
        x = static_cast<int>(dis(gen) * (_width - 1));
        z = (dis(gen) > 0.5f) ? (_depth - 1) : 0;
    }

    return glm::ivec3(x, yRange.x + static_cast<int>(dis(gen) * (yRange.y - yRange.x)), z);
}

int BlockManager::GetIndex(glm::ivec3 point) {
    return point.x + (_width * (point.z + _depth * point.y));
}

int BlockManager::GetIndex(int x, int y, int z) {
    return x + (_width * (z + _depth * y));
}

int BlockManager::GetChunkIndex(glm::ivec3 chunk) {
    return chunk.x + ((_width / _chunkSize) * (_depth / _chunkSize) * chunk.y) + chunk.z * (_width / _chunkSize);
}

int BlockManager::GetChunkIndex(int x, int y, int z) {
    return x + ((_width / _chunkSize) * (_depth / _chunkSize) * y) + z * (_width / _chunkSize);
}

bool BlockManager::CheckAdjacency(int x, int y, int z){
    return _blocksData[GetIndex(x, y, z)].IsSolid();
}

bool BlockManager::InBounds(glm::ivec3 position) {
    return position.x >= 0 && position.y >= 0 && position.z >= 0 &&
        position.x < _width && position.y < _height && position.z < _depth;
}

bool BlockManager::InBounds(int x, int y, int z) {
    return x >= 0 && y >= 0 && z >= 0 &&
        x < _width && y < _height && z < _depth;
}

bool BlockManager::InBounds(glm::ivec3 position, int margin)
{
    return position.x >= margin && position.x < (_width - margin) &&
        position.y >= margin && position.y < (_height - margin) &&
        position.z >= margin && position.z < (_depth - margin);
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

bool BlockManager::IsEdgeBlock(glm::ivec3 pos)
{
    return (pos.x == 0 || pos.x == _width - 1 || pos.y == 0 || pos.y == _height - 1 || pos.z == 0 || pos.z == _depth - 1);
}

bool BlockManager::IsEdgeBlock(BlockData& blockData)
{
    glm::ivec3 pos = blockData.GetPosID(); 

    return IsEdgeBlock(pos.x, pos.y, pos.z);
}

bool BlockManager::IsPointTooClose(const std::vector<glm::vec3>& points, const glm::vec3& candidate, float minDist)
{
    for (const auto& point : points)
    {
        if (glm::distance(point, candidate) < minDist)
        {
            return true;
        }
    }
    return false;
}


BlockData* BlockManager::GetBlock(glm::ivec3 position) {
    if (InBounds(position)) {
        int index = GetIndex(position);
        return &_blocksData[index];
    }
    else {
        return nullptr;
    }
}


void BlockManager::addToInspector(ImguiMain *imguiMain) {
    if (ImGui::TreeNode("BlockManager")) {
        // Gui related things go there
        ImGui::Text("Test1:");
        ImGui::Text("Test2:");

        ImGui::TreePop();
    }
}
