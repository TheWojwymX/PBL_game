#include "CloudManager.h"

CloudManager::CloudManager(int width, int height, int depth) :
        _width(width), _depth(depth), _height(height), _rangeRand(300, 100, 999) {}

CloudManager::CloudManager(): _rangeRand(300,100,999){
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

void CloudManager::Initiate()
{
    _sandRendererRef = COMPONENTSMANAGER.GetComponentByID<InstanceRenderer>(_sandRendererRefID);
    _cameraRef = COMPONENTSMANAGER.GetComponentByID<Camera>(_cameraRefID);
    Component::Initiate();
}

void CloudManager::Init() {
    GenerateMap();
    UpdateBlocksVisibility();
    RefreshVisibleBlocks();
    UpdateInstanceRenderer();
}

void CloudManager::GenerateMap() {
    _blocksData.clear();
    // Iterate over the grid dimensions
    for (int y = 0; y < _height; y++) {
        for (int z = 0; z < _depth; z++) {
            for (int x = 0; x < _width; x++) {
                float density = Perlin(x,y,z);

                float normalized = (density - (-3000.0)) / (3000.0 - (-3000.0));

                bool filled;
                if(normalized < 0.4f) filled = true;
                else filled = false;

                BlockType type = filled ? BlockType::SAND : BlockType::EMPTY;
                // Create BlockData object with Sand type and add it to the vector
                BlockData cloudBlock(type, glm::vec3(-2500 + (70 * x), 430, -2500 + (70 * z)), 1.0f, false, density, shared_from_this());
                _blocksData.push_back(cloudBlock);
            }
        }
    }
}

void CloudManager::UpdateInstanceRenderer() {
    std::vector<glm::vec3> instancePositions;

    // Iterate through _visibleBlocks and add visible non-empty blocks to instanceMatrix
    for (const auto& blockPtr : _visibleBlocks) {
        const BlockData& blockData = *blockPtr;

        instancePositions.push_back(blockData.GetPosID());
    }

    // Pass the instanceMatrix to _sandRendererRef
    if (_sandRendererRef) {
        _sandRendererRef->RefreshPositionBuffer(instancePositions);
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
        SetVisibility(blockData, true);
    }
}

void CloudManager::SetVisibility(BlockData& blockData, bool state) {
    // Set the visibility of the block
    blockData.SetVisible(state);

    if (state && blockData.GetBlockType() != BlockType::EMPTY) {
        // Add the blockData to _visibleBlocks
        _visibleBlocks.push_back(&blockData);
    }
}

float CloudManager::Perlin(float x, float y, float z) {
    int seed = _rangeRand.GetRandomInt();
    float scale = 20.0f; // Determines noise granularity

    float total = 0.0f;
    float persistence = 0.5f; // Amplitude decrease
    int octaves = 8;
    float frequency = scale; // Initial frequency
    float amplitude = 1.0f; // Initial amplitude

    PerlinNoise noiseGenerator(seed);

    for (int i = 0; i < octaves; ++i) {
        float nx = x * frequency;
        float ny = y * frequency;
        float nz = z * frequency;

        total += noiseGenerator.Noise(nx, ny, nz) * amplitude;

        frequency *= 2.0f; // Increase frequency each octave
        amplitude *= persistence; // Decrease amplitude each octave
    }

    // Normalize to the range [0, 1] after accumulation
    total = (total + 1) / 2;

    return total; // Resulting value between [0, 1]
}