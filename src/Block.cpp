#include "Block.h"

Block::Block(std::shared_ptr<BlockData> blockData) : _blockData(blockData) {}

std::shared_ptr<BlockData> Block::GetBlockData() const {
    return _blockData;
}
