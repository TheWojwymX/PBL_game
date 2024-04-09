#include "Block.h"

Block::Block() {}

Block::Block(std::shared_ptr<BlockData> blockData) : _blockData(blockData) {}

std::shared_ptr<BlockData> Block::GetBlockData() const {
    return _blockData;
}

void Block::addToInspector(ImguiMain *imguiMain)
{
    if (ImGui::TreeNode("Block"))
    {
        // Block Gui related things go there
        ImGui::Text("Test1:");
        ImGui::Text("Test2:");

        ImGui::TreePop();

    }
}


