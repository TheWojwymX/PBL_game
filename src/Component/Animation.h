#pragma once

#include <iostream>
#include <vector>
#include "model.h"
#include "Core/Component.h"
#include "Component/MeshRenderer.h"
#include "Managers/ComponentsManager.h"
#include "BatchRandomGenerator.h"

enum state { SPAWN, WALK, ATTACK, DEAD };

class Animation: public Component {
public:
    Animation();
    Animation(std::shared_ptr<MeshRenderer> meshRenderer, float frameDuration, bool loop, state enemyState);
    state _enemyState = WALK;
    bool _toDelete = false;


    nlohmann::json Serialize() override;
    void Deserialize(const nlohmann::json &jsonData) override;
    void Initiate() override;

    std::shared_ptr<Model> GetCurrentFrame();
    void Update();
    void Reset();
    void InitFrames(std::vector<std::vector<std::shared_ptr<Model>>> frames);
    bool IsAnimationFinished();
    void setEntityType(int type);
    void setCurrentTime(float time);
    void setFrameDuration(float frameDuration);
    void setLoop(bool loop);
    void setMeshRenderer(const shared_ptr<MeshRenderer> &meshRenderer);
    void InitComponent(int type, std::vector < std::vector<std::shared_ptr<Model>>> frames);

    void setMeshRendererId(int meshRendererId);

    //void addToInspector(ImguiMain *imguiMain) override;
private:
    std::vector<std::shared_ptr<Model>> _spawnFrames;
    std::vector<std::shared_ptr<Model>> _walkFrames;
    std::vector<std::shared_ptr<Model>> _attackFrames;
    std::vector<std::shared_ptr<Model>> _deadFrames;
    BatchRandomGenerator _randomGen;


    float _frameDuration; // Duration of each frame in seconds
    float _currentTime = 0.0f;   // Current time in the animation
    bool _loop;           // Whether the animation should loop
    std::shared_ptr<MeshRenderer> _meshRenderer;
    int _meshRendererID;
    int _enemyType;
};