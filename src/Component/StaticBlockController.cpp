#include "StaticBlockController.h"
#include "Core/InputManager.h"

StaticBlockController::StaticBlockController(std::shared_ptr<BlockData> blockData) : Block(blockData) {}

void StaticBlockController::Init() {
    std::cout << "Hello world" << std::endl;
}

void StaticBlockController::Input() {
    if(INPUT.IsKeyPressed(GLFW_KEY_SPACE))
        std::cout << "Hello world" << std::endl;
}
