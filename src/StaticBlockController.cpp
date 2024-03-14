#include "StaticBlockController.h"

StaticBlockController::StaticBlockController(std::shared_ptr<BlockData> blockData) : Block(blockData) {}

void StaticBlockController::Initialize() {
    std::cout << "Hello world" << std::endl;
}
