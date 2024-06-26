#pragma once

#include <random>
#include "Component/InstanceRenderer.h"
#include "Managers/ComponentsManager.h"
#include "Component/Camera.h"
#include "BlockData.h"
#include "BatchRandomGenerator.h"

class CloudManager : public Component, public std::enable_shared_from_this<CloudManager> {
public:
    CloudManager(int width, int height, int depth);

    CloudManager();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json& jsonData) override;

    void Initiate() override;

    void Init() override;

    void SetInstanceRenderer(std::shared_ptr<InstanceRenderer> renderer) { _sandRendererRef = renderer; }
    void SetCamera(std::shared_ptr<Camera> camera) { _cameraRef = camera; }

    static CloudManager& GetInstance() {
        static CloudManager instance; // Static instance of CloudManager
        return instance;
    }

private:
    int _width;
    int _depth;
    int _height;
    std::vector<BlockData> _blocksData;
    std::vector<BlockData*> _visibleBlocks;
    std::shared_ptr<InstanceRenderer> _sandRendererRef;
    std::shared_ptr<Camera> _cameraRef;
    std::vector<std::vector<glm::ivec3>> _sphereVectors;
    int _sandRendererRefID;
    int _cameraRefID;
    BatchRandomGenerator _rangeRand;


    void GenerateMap();
    void UpdateInstanceRenderer();
    void RefreshVisibleBlocks();
    void UpdateBlocksVisibility();
    void SetVisibility(BlockData& blockData, bool state);
    float Perlin(float x, float y, float z);
};

class PerlinNoise {
public:
    // Constructor with a given seed
    PerlinNoise(int seed) {
        // Generate a permutation table for noise generation
        p.resize(256);
        std::iota(p.begin(), p.end(), 0);
        std::shuffle(p.begin(), p.end(), std::default_random_engine(seed));
        p.insert(p.end(), p.begin(), p.end());
    }

    float Noise(float x, float y, float z) {
        int X = static_cast<int>(std::floor(x)) & 255;
        int Y = static_cast<int>(std::floor(y)) & 255;
        int Z = static_cast<int>(std::floor(z)) & 255;

        float u = Fade(x - std::floor(x));
        float v = Fade(y - std::floor(y));
        float w = Fade(z - std::floor(z));

        int A = p[X] + Y;
        int B = p[X + 1] + Y;

        int AA = p[A] + Z;
        int AB = p[A + 1] + Z;
        int BA = p[B] + Z;
        int BB = p[B + 1] + Z;

        float result = Lerp(
                Lerp(Lerp(Grad(p[AA], x, y, z), Grad(p[BA], x - 1, y, z), u),
                     Lerp(Grad(p[AB], x, y - 1, z), Grad(p[BB], x - 1, y - 1, z), u), v),
                Lerp(Lerp(Grad(p[AA + 1], x, y, z - 1), Grad(p[BA + 1], x - 1, y, z - 1), u),
                     Lerp(Grad(p[AB + 1], x, y - 1, z - 1), Grad(p[BB + 1], x - 1, y - 1, z - 1), u), v), w);

        return (result + 1.0f) / 2.0f; // Normalize to [0, 1]
    }

private:
    std::vector<int> p; // Permutation table

    inline float Fade(float t) {
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    inline float Lerp(float a, float b, float t) {
        return a + t * (b - a);
    }

    inline float Grad(int hash, float x, float y, float z) {
        int h = hash & 15;
        float u = (h < 8) ? x : y;
        float v = (h < 4) ? y : ((h == 12 || h == 14) ? x : z);
        return ((h & 1) ? -u : u) + ((h & 2) ? -v : v);
    }
};