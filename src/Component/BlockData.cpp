#include "BlockData.h"

BlockData::BlockData(BlockType blockType, int xID, int yID, int zID, float startHP, bool invincible, std::shared_ptr<BlockManager> blockManager)
    : _blockType(blockType), _xID(xID), _yID(yID), _zID(zID), _HP(startHP), _startHP(startHP), _invincible(invincible), _blockManager(blockManager), _visible(true) {}

BlockType BlockData::GetBlockType() const {
    return _blockType;
}

int BlockData::GetXID() const {
    return _xID;
}

int BlockData::GetYID() const {
    return _yID;
}

int BlockData::GetZID() const {
    return _zID;
}

float BlockData::GetHP() const {
    return _HP;
}

float BlockData::GetStartHP() const {
    return _startHP;
}

bool BlockData::IsInvincible() const {
    return _invincible;
}

std::shared_ptr<BlockManager> BlockData::GetBlockManager() const {
    return _blockManager;
}

bool BlockData::IsVisible() const {
    return _visible;
}

void BlockData::SetBlockType(BlockType blockType) {
    _blockType = blockType;
}

void BlockData::SetXID(int xID) {
    _xID = xID;
}

void BlockData::SetYID(int yID) {
    _yID = yID;
}

void BlockData::SetZID(int zID) {
    _zID = zID;
}

void BlockData::SetHP(float hp) {
    _HP = hp;
}

void BlockData::SetStartHP(float startHP) {
    _startHP = startHP;
}

void BlockData::SetInvincible(bool invincible) {
    _invincible = invincible;
}

void BlockData::SetBlockManager(std::shared_ptr<BlockManager> sandManager) {
    _blockManager = sandManager;
}

void BlockData::SetVisible(bool visible) {
    _visible = visible;
}
