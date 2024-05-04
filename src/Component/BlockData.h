#pragma once

#include <memory>
#include <glm/glm.hpp>

class BlockManager;

enum class BlockType {
    EMPTY,
    DIRT,
    SAND
};

class BlockData {
public:
    BlockData(BlockType blockType, glm::ivec3 posID, glm::mat4 matrix, float startHP, bool invincible, std::weak_ptr<BlockManager> blockManager);

    BlockType GetBlockType() const { return _blockType; }
    glm::ivec3 GetPosID() const { return _posID; }
    float GetHP() const { return _HP; }
    float GetStartHP() const { return _startHP; }
    bool IsInvincible() const { return _invincible; }
    std::weak_ptr<BlockManager> GetBlockManager() const { return _blockManager; }
    bool IsVisible() const { return _visible; }
    glm::mat4 GetMatrix() const { return _matrix; }
    bool GetSideCollision(int index) const { return _sideCollisions[index]; }

    void SetBlockType(BlockType blockType) { _blockType = blockType; }
    void SetPosID(glm::ivec3 posID) { _posID = posID; }
    void SetHP(float hp) { _HP = hp; }
    void SetStartHP(float startHP) { _startHP = startHP; }
    void SetInvincible(bool invincible) { _invincible = invincible; }
    void SetBlockManager(std::weak_ptr<BlockManager> blockManager) { _blockManager = blockManager; }
    void SetVisible(bool visible) { _visible = visible; }
    void SetMatrix(const glm::mat4& matrix) { _matrix = matrix; }
    void SetSideCollisions(bool right, bool left, bool top, bool bottom, bool front, bool back) {
        _sideCollisions[0] = right;
        _sideCollisions[1] = left;
        _sideCollisions[2] = top;
        _sideCollisions[3] = bottom;
        _sideCollisions[4] = front;
        _sideCollisions[5] = back;
    }

private:
    BlockType _blockType;
    glm::ivec3 _posID;
    float _HP;
    float _startHP;
    bool _invincible;
    std::weak_ptr<BlockManager> _blockManager;
    bool _visible;
    glm::mat4 _matrix;
    bool _sideCollisions[6];
};
