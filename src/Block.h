#pragma once

#include "BlockData.h"
#include "Component.h"
#include <memory>

class Block : public Component {
public:
    Block();
    Block(std::shared_ptr<BlockData> blockData);

    std::shared_ptr<BlockData> GetBlockData() const;

protected:
    std::shared_ptr<BlockData> _blockData;
};
