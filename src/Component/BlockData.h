#pragma once

#include <memory>
#include <glm/glm.hpp>
#include <variant>
#include <vector>
#include <iostream>

class BlockManager;
class CloudManager;
class GlowstickMovement;

enum class BlockType {
    EMPTY,
    DIRT,
    SAND,
    CLOUD,
    PLASTIC,
    METAL
};

class BlockData {
public:
    BlockData(BlockType blockType, glm::ivec3 posID, float startHP, bool invincible, float density, std::variant<std::weak_ptr<BlockManager>, std::weak_ptr<CloudManager>> blockManager);

    BlockType GetBlockType() const { return _blockType; }
    glm::ivec3 GetPosID() const { return _posID; }
    float GetHP() const { return _HP; }
    float GetStartHP() const { return _startHP; }
    bool IsInvincible() const { return _invincible; }
    float GetDensity() const { return _density; }
    std::variant<std::weak_ptr<BlockManager>, std::weak_ptr<CloudManager>> GetBlockManager() const { return _blockManager; }
    bool IsVisible() const { return _visible; }
    bool IsRendered() const { return _rendered; }
    bool IsSolid() const { return _blockType != BlockType::EMPTY; }
    bool IsMaterial() const { return _blockType == BlockType::PLASTIC || _blockType == BlockType::METAL; }
    void AddGlowstick(std::shared_ptr<GlowstickMovement> glowstick) { _stuckGlowsticks.push_back(glowstick); }
    glm::ivec3 GetChunkID(int chunkSize) const { return glm::ivec3(_posID.x / chunkSize, _posID.y / chunkSize, _posID.z / chunkSize); }
    bool GetCollSide(int side) const { return _collSides[side]; }

    void SetBlockType(BlockType blockType) { _blockType = blockType; }
    void SetPosID(glm::ivec3 posID) { _posID = posID; }
    void SetHP(float hp) { _HP = hp; }
    void SetStartHP(float startHP) { _startHP = startHP; }
    void SetInvincible(bool invincible) { _invincible = invincible; }
    void SetBlockManager(std::variant<std::weak_ptr<BlockManager>, std::weak_ptr<CloudManager>> blockManager) { _blockManager = blockManager; }
    void SetVisible(bool visible) { _visible = visible; }
    void SetRendered(bool rendered) { _rendered = rendered; }
    void SetCollSides(bool negX, bool posX, bool negY, bool posY, bool negZ, bool posZ) {
        _collSides[0] = negX;
        _collSides[1] = posX;
        _collSides[2] = negY;
        _collSides[3] = posY;
        _collSides[4] = negZ;
        _collSides[5] = posZ;
    }

    void UnstuckGlowsticks();
    bool DamageBlock(float amount);
    void HandleParticles();

private:
    BlockType _blockType;
    glm::ivec3 _posID;
    float _HP;
    float _startHP;
    bool _invincible;
    float _density;
    std::variant<std::weak_ptr<BlockManager>, std::weak_ptr<CloudManager>> _blockManager;
    bool _visible;
    bool _rendered;
    std::vector<std::shared_ptr<GlowstickMovement>> _stuckGlowsticks;
    bool _collSides[6];
};
