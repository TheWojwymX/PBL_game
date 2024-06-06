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
    GenerateMap(0.5f,7);
    GenerateTopLayer(glm::ivec2(50,50),glm::ivec2(500,500),glm::ivec2(50,50));
    GenerateSphereVectors(31);
    GenerateResources();
    UpdateBlocksVisibility();
    RefreshVisibleBlocks();
    UpdateRenderedChunks();
}

void BlockManager::UpdateInstanceRenderer() {
    std::vector<glm::mat4> instancedSandMatrix;
    std::vector<glm::mat4> instancedPlasticMatrix;
    std::vector<glm::mat4> instancedMetalMatrix;

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
                    instancedSandMatrix.push_back(blockPtr->GetMatrix());
                    break;
                case BlockType::PLASTIC:
                    instancedPlasticMatrix.push_back(blockPtr->GetMatrix());
                    break;
                case BlockType::METAL:
                    instancedMetalMatrix.push_back(blockPtr->GetMatrix());
            }
        }
    }

    // Pass the instanceMatrix to _sandRendererRef
    if (_sandRendererRef) {
        _sandRendererRef->SetInstanceMatrix(instancedSandMatrix);
    }
    if (_plasticRendererRef) {
        _plasticRendererRef->SetInstanceMatrix(instancedPlasticMatrix);
    }
    if (_metalRendererRef) {
        _metalRendererRef->SetInstanceMatrix(instancedMetalMatrix);
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


std::vector<CollisionInfo> BlockManager::CalculateCollisionInfo(glm::vec3 entityPos, glm::vec3 movementVector, float halfWidth, float entityHeight) {
    std::vector<CollisionInfo> collisionInfoList;

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
        glm::ivec3 roundedPosX = glm::round(glm::vec3(corners[i].x + movementVector.x, corners[i].y, corners[i].z));
        glm::ivec3 roundedPosY = glm::round(glm::vec3(corners[i].x, corners[i].y + movementVector.y, corners[i].z));
        glm::ivec3 roundedPosZ = glm::round(glm::vec3(corners[i].x, corners[i].y, corners[i].z + movementVector.z));

        // Initialize CollisionInfo for the current corner
        CollisionInfo info;

        // Check if the rounded position is within bounds
        if (InBounds(roundedPosX)) {
            int index = GetIndex(roundedPosX);

            // Check if the block at the index is not empty
            if (_blocksData[index].IsSolid()) {
                // Calculate the AABB extents of the block
                glm::vec3 blockMin = glm::vec3(roundedPosX) - glm::vec3(0.5f);
                glm::vec3 blockMax = blockMin + glm::vec3(1.0f);

                float entityPosX = entityPos.x + movementVector.x;
                if (entityPosX + halfWidth > blockMin.x || entityPosX - halfWidth < blockMax.x) {
                    // Determine the separation vector in the x-axis
                    float direction = glm::sign(entityPosX - roundedPosX.x);
                    info.separationVector.x = (std::abs((std::abs(entityPosX - roundedPosX.x) - (halfWidth + 0.5f))) + 0.0001f) * direction;
                    info.isColliding = true;
                }
            }
        }

        if (InBounds(roundedPosY)) {
            int index = GetIndex(roundedPosY);

            // Check if the block at the index is not empty
            if (_blocksData[index].IsSolid()) {
                // Calculate the AABB extents of the block
                glm::vec3 blockMin = glm::vec3(roundedPosY) - glm::vec3(0.5f);
                glm::vec3 blockMax = blockMin + glm::vec3(1.0f);

                float entityPosY = entityPos.y + movementVector.y;
                if (i < 4) {
                    if (entityPosY < blockMax.y) {
                        info.separationVector.y = (std::abs((std::abs(entityPosY - blockMax.y))) + 0.0001f);
                        info.isColliding = true;
                    }
                }
                else if (i >= (corners.size() - 4)) {
                    if (entityPosY + entityHeight > blockMin.y) {
                        info.separationVector.y = (std::abs((std::abs(entityPosY + entityHeight - blockMin.y))) + 0.0001f) * -1;
                        info.isColliding = true;
                    }
                }
            }
        }

        if (InBounds(roundedPosZ)) {
            int index = GetIndex(roundedPosZ);

            // Check if the block at the index is not empty
            if (_blocksData[index].IsSolid()) {
                // Calculate the AABB extents of the block
                glm::vec3 blockMin = glm::vec3(roundedPosZ) - glm::vec3(0.5f);
                glm::vec3 blockMax = blockMin + glm::vec3(1.0f);

                float entityPosZ = entityPos.z + movementVector.z;
                if (entityPosZ + halfWidth > blockMin.z || entityPosZ - halfWidth < blockMax.z) {
                    // Determine the separation vector in the z-axis
                    float direction = glm::sign(entityPosZ - roundedPosZ.z);
                    info.separationVector.z = (std::abs((std::abs(entityPosZ - roundedPosZ.z) - (halfWidth + 0.5f))) + 0.0001f) * direction;
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

    // Apply the mask using _entranceMask
    ApplyMask(glm::ivec3(centerX-3, centerY, centerZ-3), _entranceMask, _entranceMaskDimensions);

    ApplyMask(glm::ivec3(centerX - 11, centerY-5, centerZ - 12), _tutorialCaveMask, _tutorialCaveMaskDimensions);
}

void BlockManager::GenerateTopLayer(glm::ivec2 center, glm::ivec2 dimensions, glm::ivec2 deadzone)
{
    std::vector<glm::mat4> instanceMatrix;
    float frequency = 0.025f; // Frequency of the noise (controls the detail level)
    float amplitude = 10.0f; // Amplitude of the noise (controls the height variation)
    float smoothEnd = 0.35f;

    int startX = center.x - dimensions.x / 2;
    int startZ = center.y - dimensions.y / 2;

    // Calculate the maximum distance from the edge of the deadzone to the point where full noise height is applied
    int maxDistanceX = dimensions.x / 2 - deadzone.x;
    int maxDistanceZ = dimensions.y / 2 - deadzone.y;

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

            // Generate a height value using Perlin noise
            float height = glm::perlin(glm::vec2(x * frequency, z * frequency)) * amplitude;

            float smoothingFactor = glm::clamp(float(distX + distZ) / float((maxDistanceX + maxDistanceZ) / (1 / smoothEnd)), 0.0f, 1.0f);

            height *= smoothingFactor;

            // Calculate transform matrix for the current block with noise-adjusted height
            glm::mat4 transformMatrix = Transform::CalculateTransformMatrix(glm::vec3(x, (_height - 1) + height, z), glm::quat(), glm::vec3(1.0f));

            // Add the block to the vector
            instanceMatrix.push_back(transformMatrix);
        }
    }

    // Iterate over _blocksData and add edge blocks to instanceMatrix
    for (BlockData& blockData : _blocksData) {
        if (IsEdgeBlock(blockData) && blockData.IsSolid()) {
            instanceMatrix.push_back(blockData.GetMatrix());
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
                BlockType type = filled ? BlockType::DIRT : BlockType::EMPTY;
                float hp = 0.0f;
                if (type == BlockType::DIRT) {
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

    
    movementVector += separationVector;

    // Return the separation vector
    return std::make_pair(movementVector,separationVector);
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

            if (entityPos.x > blockMin.x || entityPos.x < blockMax.x) {
                float direction = glm::sign(entityPos.x - roundedPos.x);
                separationVector.x = (std::abs((std::abs(entityPos.x - roundedPos.x) - 0.5f)) + 0.0001f) * direction;
            }

            if (entityPos.y < blockMax.y) {
                separationVector.y = (std::abs((std::abs(entityPos.y - blockMax.y))) + 0.0001f);
            }
            else if (entityPos.y > blockMin.y) {
                separationVector.y = (std::abs((std::abs(entityPos.y - blockMin.y))) + 0.0001f) * -1;
            }

            if (entityPos.z > blockMin.z || entityPos.z < blockMax.z) {
                float direction = glm::sign(entityPos.z - roundedPos.z);
                separationVector.z = (std::abs((std::abs(entityPos.z - roundedPos.z) - 0.5f)) + 0.0001f) * direction;
            }

            // Find the smallest component of the separation vector
            float smallestComponent = std::min(std::min(separationVector.x, separationVector.y), separationVector.z);

            // Set other components to zero except the smallest one
            if (smallestComponent == separationVector.x) {
                separationVector.y = 0.0f;
                separationVector.z = 0.0f;
            }
            else if (smallestComponent == separationVector.y) {
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

glm::vec3 BlockManager::GetCaveFloor(glm::vec3 entityPos)
{
    glm::ivec3 roundedPos = glm::round(entityPos);

    return glm::vec3();
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
        case BlockType::METAL:
        case BlockType::PLASTIC:
        case BlockType::DIRT:
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

    // Define the probability of a block being changed to METAL or PLASTIC
    const double metalProbability = .5;   // 50% chance of being METAL
    const double plasticProbability = .5; // 50% chance of being PLASTIC

    // Generate Poisson disk distribution points
    std::vector<glm::vec3> poissonPoints = GeneratePoissonDiskPoints();
    std::cout <<"Materials: "<< poissonPoints.size() << std::endl;
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
                if (randomValue < metalProbability)
                {
                    ChangeType(_blocksData[index], BlockType::METAL);
                    break;
                }
                // Check if the block should be changed to PLASTIC
                else if (randomValue < (metalProbability + plasticProbability))
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
            UpdateNeighbourVisibility(blockData);
    }
}

int BlockManager::DestroyBlock(BlockData& blockData)
{
    blockData.SetBlockType(BlockType::EMPTY);
    UpdateNeighbourVisibility(blockData);
    blockData.UnstuckGlowsticks();
    return GetChunkIndex(blockData.GetChunkID(_chunkSize));
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
