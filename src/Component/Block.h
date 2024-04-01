#pragma once

#include "BlockData.h"
#include "Core/Component.h"
#include <memory>
#include "imgui.h"

class Block : public Component {
public:
    Block();
    Block(std::shared_ptr<BlockData> blockData);

    std::shared_ptr<BlockData> GetBlockData() const;

    void addToInspector(ImguiMain *imguiMain) override;

protected:
    std::shared_ptr<BlockData> _blockData;
};
