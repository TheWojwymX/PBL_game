#include "CloudManager.h"

CloudManager::CloudManager(int width, int height, int depth) :
        _width(width), _depth(depth), _height(height) {}

CloudManager::CloudManager() {
    _type = ComponentType::CLOUDMANAGER;
}

nlohmann::json CloudManager::Serialize() {
    nlohmann::json data = Component::Serialize();

    data["width"] = _width;
    data["depth"] = _depth;
    data["height"] = _height;

    if(_sandRendererRef){
        data["sandRendererRefID"] = _sandRendererRef->_id;
    }

    if(_cameraRef){
        data["cameraRefID"] = _cameraRef->_id;
    }

    return data;
}

void CloudManager::Deserialize(const nlohmann::json &jsonData) {

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

    if (jsonData.contains("cameraRefID")) {
        _cameraRefID = jsonData["cameraRefID"].get<int>();
    }

    Component::Deserialize(jsonData);
}

void CloudManager::initiate()
{
    _sandRendererRef = COMPONENTSMANAGER.GetComponentByID<InstanceRenderer>(_sandRendererRefID);
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(_cameraRefID);
    Component::initiate();
}

void CloudManager::Init() {
    GenerateMap();
    UpdateBlocksVisibility();
    RefreshVisibleBlocks();
    UpdateInstanceRenderer();
    //MoveAllBlocks(glm::vec3(-200, 160, -240));
}

void CloudManager::Refresh() {
    UpdateBlocksVisibility();
    RefreshVisibleBlocks();
    UpdateInstanceRenderer();
}

void CloudManager::GenerateMap() {
    _blocksData.clear();
    float lowestValue = 1;
    // Iterate over the grid dimensions
    for (int y = 0; y < _height; y++) {
        for (int z = 0; z < _depth; z++) {
            for (int x = 0; x < _width; x++) {
                //float densityCutoff = RandomInRange(-2.8f, 1.5f);
                float density = Perlin(x,y,z);
                //density = std::clamp(density, 0.0f, 1.0f);
                if(density == 1.0f)
                {
                    density = density - 0.1;
                }
                //std::cout << density << " " << densityCutoff << std::endl;
                // Calculate transform matrix for the current block
                glm::mat4 transformMatrix = Transform::CalculateTransformMatrix(glm::vec3(-2500 + (14 * x), 180, -2500 + (14 * z)), glm::quat(), glm::vec3(14.0f,1.5f,14.0f));

                // Create BlockData object with Sand type and add it to the vector
                BlockData cloudBlock(BlockType::SAND, glm::ivec3(x, y, z), transformMatrix, 1.0f, false, density, shared_from_this());
                _blocksData.push_back(cloudBlock);
            }
        }
    }
}

void CloudManager::UpdateInstanceRenderer() {
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

void CloudManager::RefreshVisibleBlocks() {
    // Use erase-remove idiom to remove elements satisfying the condition
    _visibleBlocks.erase(std::remove_if(_visibleBlocks.begin(), _visibleBlocks.end(),
                                        [](BlockData* blockPtr) {
                                            // Remove the block if it's either empty or not visible
                                            return blockPtr->GetBlockType() == BlockType::EMPTY || !blockPtr->IsVisible();
                                        }), _visibleBlocks.end());
}

void CloudManager::UpdateBlocksVisibility() {
    _visibleBlocks.clear(); // Clear the list of visible blocks

    // Iterate through all blocks
    for (auto& blockData : _blocksData) {

        float densityCutoff = RandomInRange(-2.0f, 1.1f);
        if(blockData.GetDensity() < densityCutoff) {
            UpdateBlockVisibility(blockData);
        }
    }
}

void CloudManager::UpdateBlockVisibility(BlockData& blockData)
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


void CloudManager::SetVisibility(BlockData& blockData, bool state) {
    // Set the visibility of the block
    blockData.SetVisible(state);

    if (state && blockData.GetBlockType() != BlockType::EMPTY) {
        // Add the blockData to _visibleBlocks
        _visibleBlocks.push_back(&blockData);
    }
}

int CloudManager::GetIndex(glm::ivec3 point) {
    return point.x + (_width * _depth * point.y) + point.z * _width;
}

int CloudManager::GetIndex(int x, int y, int z) {
    return x + (_width * _depth * y) + z * _width;
}


bool CloudManager::CheckAdjacency(int x, int y, int z)
{
    return _blocksData[GetIndex(x, y, z)].GetBlockType() != BlockType::EMPTY;
}

bool CloudManager::InBounds(glm::ivec3 position) {
    return position.x >= 0 && position.y >= 0 && position.z >= 0 &&
           position.x < _width && position.y < _height && position.z < _depth;
}

void CloudManager::addToInspector(ImguiMain *imguiMain)
{
    ImGui::Text(" Siema jestem blok manager ");
}

void CloudManager::MoveAllBlocks(const glm::vec3& offset) {
    // Iterate through all blocks and update their positions
    for (auto& blockData : _blocksData) {
        // Get the current position of the block
        glm::ivec3 currentPosition = blockData.GetPosID();

        // Update the position of the block by adding the offset
        glm::ivec3 newPosition = currentPosition + glm::ivec3(offset);

        // Update the position of the block in BlockData
        blockData.SetPosID(newPosition);

        // Update the transform matrix of the block
        glm::mat4 newTransform = Transform::CalculateTransformMatrix(glm::vec3(newPosition), glm::quat(), glm::vec3(1.0f));
        blockData.SetMatrix(newTransform);
    }

    // After moving all blocks, update any visual representation/rendering
    UpdateInstanceRenderer();
}

void CloudManager::Update()
{

    //MoveAllBlocks(glm::vec3(1, 0, 0));
    //std::cout << _blocksData[GetIndex(0,0,0)].GetDensity() << " " << _blocksData[GetIndex(1,0,0)].GetDensity() << " " << _blocksData[GetIndex(2,0,0)].GetDensity() << std::endl;
}

float CloudManager::Perlin(float x, float y, float z) {
    int seed = 123;
    float scale = 0.11f;

    float total = 0.0f;
    float persistence = 0.2f;
    int octaves = 8;
    float frequency = scale;
    float amplitude = 1.0f;

    for (int i = 0; i < octaves; ++i) {
        // Offset the coordinates for each octave
        float offsetX = x * frequency;
        float offsetY = y * frequency;
        float offsetZ = z * frequency;

        total += std::sin((offsetX + seed) * frequency) * amplitude;
        total += std::sin((offsetY + seed) * frequency) * amplitude;
        total += std::sin((offsetZ + seed) * frequency) * amplitude;

        frequency *= 2.0f;
        amplitude *= persistence;
    }

    return total;
}

float CloudManager::RandomInRange(float min, float max) {
    std::random_device rd; // Obtain a random number from hardware
    std::mt19937 eng(rd()); // Seed the generator
    std::uniform_real_distribution<float> distr(min, max); // Define the range

    return distr(eng); // Generate and return a random number within the specified range
}