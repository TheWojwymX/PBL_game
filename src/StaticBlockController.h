#pragma once
#include "Block.h"

class StaticBlockController : public Block
{
public:
    StaticBlockController(std::shared_ptr<BlockData> blockData);

    void Initialize() override;
};
