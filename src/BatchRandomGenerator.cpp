#include "BatchRandomGenerator.h"

BatchRandomGenerator::BatchRandomGenerator(size_t batchSize, float lowerBound, float upperBound)
    : _gen(_rd()), _floatDis(lowerBound, upperBound), _batchSize(batchSize), _isIntDistribution(false) {
    GenerateFloatBatch();
}

BatchRandomGenerator::BatchRandomGenerator(size_t batchSize, int lowerBound, int upperBound)
    : _gen(_rd()), _intDis(lowerBound, upperBound), _batchSize(batchSize), _isIntDistribution(true) {
    GenerateIntBatch();
}

float BatchRandomGenerator::GetRandomFloat() {
    if (_floatNumbers.empty()) {
        GenerateFloatBatch();
    }
    float number = _floatNumbers.front();
    _floatNumbers.pop();
    return number;
}

int BatchRandomGenerator::GetRandomInt() {
    if (_intNumbers.empty()) {
        GenerateIntBatch();
    }
    int number = _intNumbers.front();
    _intNumbers.pop();
    return number;
}

void BatchRandomGenerator::GenerateFloatBatch() {
    for (size_t i = 0; i < _batchSize; ++i) {
        _floatNumbers.push(_floatDis(_gen));
    }
}

void BatchRandomGenerator::GenerateIntBatch() {
    for (size_t i = 0; i < _batchSize; ++i) {
        _intNumbers.push(_intDis(_gen));
    }
}
