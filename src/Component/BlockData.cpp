#include "BlockData.h"

BlockData::BlockData(BlockType blockType, glm::ivec3 posID, glm::mat4 matrix, float startHP, bool invincible, float density, std::variant<std::weak_ptr<BlockManager>, std::weak_ptr<CloudManager>> blockManager)
    : _blockType(blockType), _posID(posID), _matrix(matrix), _HP(startHP), _startHP(startHP), _invincible(invincible), _density(density), _blockManager(blockManager), _visible(true), _sideCollisions{ false, false, false, false, false, false } {}
