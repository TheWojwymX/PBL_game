#include "BlockData.h"
#include "Component/GlowstickMovement.h"

BlockData::BlockData(BlockType blockType, glm::ivec3 posID, float startHP, bool invincible, float density, std::variant<std::weak_ptr<BlockManager>, std::weak_ptr<CloudManager>> blockManager)
    : _blockType(blockType), _posID(posID), _startHP(startHP), _HP(startHP), _invincible(invincible), _density(density), _blockManager(blockManager), _visible(true), _rendered(false) {
}

void BlockData::UnstuckGlowsticks()
{
    // Iterate through the vector of stuck glowsticks
    for (auto& glowstick : _stuckGlowsticks)
    {
        glowstick->ResumeFall();
    }

    // Clear the vector of stuck glowsticks
    _stuckGlowsticks.clear();
}

bool BlockData::DamageBlock(float amount) {
    if (_invincible) {
        return false; // Block is invincible, no damage taken
    }

    _HP -= amount;
    return _HP <= 0;
}
