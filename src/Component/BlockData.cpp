#include "BlockData.h"

BlockData::BlockData(BlockType blockType, glm::ivec3 posID, glm::mat4 matrix, float startHP, bool invincible, float density, std::variant<std::weak_ptr<BlockManager>, std::weak_ptr<CloudManager>> blockManager)
    : _blockType(blockType), _posID(posID), _matrix(matrix), _startHP(startHP), _HP(startHP), _invincible(invincible), _density(density), _blockManager(blockManager), _visible(true), _rendered(false) {
    std::fill(std::begin(_sideCollisions), std::end(_sideCollisions), false);
}

bool BlockData::DamageBlock(float amount) {
    if (_invincible) {
        return false; // Block is invincible, no damage taken
    }

    _HP -= amount;
    return _HP <= 0;
}
