#include "Block.h"

Block::Block() : _x(0), _y(0), _z(0), _type(BlockType::EMPTY), _invincible(false), _hp(1) {}

Block::Block(int x, int y, int z, BlockType type, int hp) : _x(x), _y(y), _z(z), _type(type), _invincible(false), _hp(hp) {}

Block::~Block() {}

int Block::GetX() const { return _x; }

int Block::GetY() const { return _y; }

int Block::GetZ() const { return _z; }

BlockType Block::GetType() const { return _type; }

void Block::SetX(int x) { _x = x; }

void Block::SetY(int y) { _y = y; }

void Block::SetZ(int z) { _z = z; }

void Block::SetType(BlockType type) { _type = type; }

bool Block::IsInvincible() const { return _invincible; }

void Block::SetInvincible(bool invincible) { _invincible = invincible; }

int Block::GetHp() const { return _hp; }

void Block::SetHp(int hp) { _hp = hp; }