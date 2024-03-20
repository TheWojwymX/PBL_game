#pragma once

#include <memory>

class BlockManager;

enum class BlockType {
    EMPTY,
    DIRT,
    SAND
};

class BlockData {
public:
    BlockData(BlockType blockType, int xID, int yID, int zID, float startHP, bool invincible, std::weak_ptr<BlockManager> blockManager);

    BlockType GetBlockType() const;
    int GetXID() const;
    int GetYID() const;
    int GetZID() const;
    float GetHP() const;
    float GetStartHP() const;
    bool IsInvincible() const;
    std::weak_ptr<BlockManager> GetBlockManager() const;
    bool IsVisible() const;

    void SetBlockType(BlockType blockType);
    void SetXID(int xID);
    void SetYID(int yID);
    void SetZID(int zID);
    void SetHP(float hp);
    void SetStartHP(float startHP);
    void SetInvincible(bool invincible);
    void SetBlockManager(std::weak_ptr<BlockManager> blockManager);
    void SetVisible(bool visible);

private:
    BlockType _blockType;
    int _xID;
    int _yID;
    int _zID;
    float _HP;
    float _startHP;
    bool _invincible;
    std::weak_ptr<BlockManager> _blockManager;
    bool _visible;
};