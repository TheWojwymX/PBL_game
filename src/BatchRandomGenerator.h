#pragma once

#include <queue>
#include <random>

class BatchRandomGenerator {
public:
    // Constructor for floating-point distribution
    BatchRandomGenerator(size_t batchSize, float lowerBound, float upperBound);

    // Constructor for integer distribution
    BatchRandomGenerator(size_t batchSize, int lowerBound, int upperBound);

    float GetRandomFloat();
    int GetRandomInt();

private:
    void GenerateFloatBatch();
    void GenerateIntBatch();

    std::random_device _rd;
    std::mt19937 _gen;
    std::uniform_real_distribution<float> _floatDis;
    std::uniform_int_distribution<int> _intDis;
    std::queue<float> _floatNumbers;
    std::queue<int> _intNumbers;
    size_t _batchSize;
    bool _isIntDistribution;
};

