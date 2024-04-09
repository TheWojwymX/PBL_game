#include "StaticBlockController.h"

StaticBlockController::StaticBlockController(std::shared_ptr<BlockData> blockData) : Block(blockData) {}

void StaticBlockController::Init() {
}

void StaticBlockController::addToInspector(ImguiMain *imguiMain)
{
    if (ImGui::TreeNode("Static Block Controller"))
    {
        // Gui related things go there
        ImGui::Text("Test1:");
        ImGui::Text("Test2:");

        ImGui::TreePop();

    }
}
