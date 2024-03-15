#pragma once
#include "Core/Component.h"

class BlockManager : public Component {
public:
    BlockManager(int width, int depth, int height); // Constructor with width, depth, and height parameters
    void GenerateMap();
private:
    int _width; 
    int _depth; 
    int _height; 
};
