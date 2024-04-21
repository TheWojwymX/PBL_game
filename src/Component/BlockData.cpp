#include "BlockData.h"

BlockData::BlockData(BlockType blockType, glm::ivec3 posID, glm::mat4 matrix, float startHP, bool invincible, std::weak_ptr<BlockManager> blockManager)
    : _blockType(blockType), _posID(posID), _matrix(matrix), _HP(startHP), _startHP(startHP), _invincible(invincible), _blockManager(blockManager), _visible(true), _sideCollisions{ false, false, false, false, false, false } {}
