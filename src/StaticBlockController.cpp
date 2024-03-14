#include "StaticBlockController.h"

// Constructor definition
StaticBlockController::StaticBlockController(std::shared_ptr<BlockData> blockData) : Block(blockData) {}

// Initialization function definition
void StaticBlockController::Initialize() {
    std::cout << "Hello world" << std::endl;
}
