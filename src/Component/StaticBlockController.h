#pragma once
#include "Component/Block.h"

class StaticBlockController : public Block
{
public:
    StaticBlockController(std::shared_ptr<BlockData> blockData);

    void Init() override;
};
