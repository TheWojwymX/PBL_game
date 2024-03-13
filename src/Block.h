#pragma once

enum class BlockType {
    EMPTY,
    DIRT,
    SAND
};

class Block {
protected:
    int _x;
    int _y;
    int _z;
    BlockType _type;
    bool _invincible;
    int _hp;

public:
    Block();
    Block(int x, int y, int z, BlockType type, int hp);
    ~Block();

    int GetX() const;
    int GetY() const;
    int GetZ() const;
    BlockType GetType() const;
    bool IsInvincible() const;
    int GetHp() const;

    void SetX(int x);
    void SetY(int y);
    void SetZ(int z);
    void SetType(BlockType type);
    void SetInvincible(bool invincible);
    void SetHp(int hp);
};
