#include "BlockData.h"
#include "Component/GlowstickMovement.h"

BlockData::BlockData(BlockType blockType, glm::ivec3 posID, float startHP, bool invincible, float density, std::variant<std::weak_ptr<BlockManager>, std::weak_ptr<CloudManager>> blockManager)
    : _blockType(blockType), _posID(posID), _startHP(startHP), _HP(startHP), _invincible(invincible), _density(density), _blockManager(blockManager), _visible(true), _rendered(false) {
}

BlockData::~BlockData() {
    // Clear the vector of stuck glowsticks
    _stuckGlowsticks.clear();
}

void BlockData::UnstuckGlowsticks()
{
    // Iterate through the vector of stuck glowsticks
    for (auto& glowstick : _stuckGlowsticks)
    {
        glowstick->ResumeFall();
    }

    // Clear the vector of stuck glowsticks
    _stuckGlowsticks.clear();
}

bool BlockData::DamageBlock(float amount) {
    if (_invincible) {
        return false; // Block is invincible, no damage taken
    }

    _HP -= amount;

    HandleParticles();

    return _HP <= 0;
}

void BlockData::HandleParticles() {
    string nodeName = to_string(_posID.x) + to_string(_posID.y) + to_string(_posID.z);
    if(NODESMANAGER.getNodeByName(nodeName) == nullptr && (GetBlockType() == BlockType::PLASTIC || GetBlockType() == BlockType::PLASTIC_BIG)) {
        NODESMANAGER.createNode(NODESMANAGER.getNodeByName("Sandbags"), nodeName);
        NODESMANAGER.getNodeByName(nodeName)->GetTransform()->SetPosition(_posID);

        string particleType;
        if(GetBlockType() == BlockType::PLASTIC){particleType = "digPlastic";}
        else {particleType = "digMetal";}

        auto hitResourceParticles = COMPONENTSMANAGER.CreateComponent<ParticleGenerator>(RESOURCEMANAGER.GetShaderByName("particleShader"), particleType);
        hitResourceParticles->SetOffset(glm::vec3(0.0f, 0.0f, 0.0f));
        hitResourceParticles->object = NODESMANAGER.getNodeByName(nodeName);

        hitResourceParticles->Init();
        NODESMANAGER.getNodeByName(nodeName)->AddComponent(hitResourceParticles);

        if(_HP > 0) hitResourceParticles->SpawnParticles();
        else{
            NODESMANAGER.getNodeByName(nodeName)->GetComponent<ParticleGenerator>()->tooltipShrink = true;
            NODESMANAGER.getNodeByName(nodeName)->GetComponent<ParticleGenerator>()->SpawnParticles();
        }
    }
    else if(GetBlockType() == BlockType::PLASTIC || GetBlockType() == BlockType::PLASTIC_BIG)
    {
        if(_HP > 0) NODESMANAGER.getNodeByName(nodeName)->GetComponent<ParticleGenerator>()->SpawnParticles();
        else{
            NODESMANAGER.getNodeByName(nodeName)->GetComponent<ParticleGenerator>()->tooltipShrink = true;
            NODESMANAGER.getNodeByName(nodeName)->GetComponent<ParticleGenerator>()->SpawnParticles();
        }
    }
}