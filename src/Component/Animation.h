#pragma once

#include <iostream>
#include <vector>
#include "model.h"
#include "Core/Component.h"
#include "Component/MeshRenderer.h"
#include "Managers/ComponentsManager.h"

class Animation: public Component {
private:
    std::vector<std::shared_ptr<Model>> _frames;
    float _frameDuration; // Duration of each frame in seconds
    float _currentTime = 0.0f;   // Current time in the animation
    bool _loop;           // Whether the animation should loop
    std::shared_ptr<MeshRenderer> _meshRenderer;
    int _meshRendererID;
    int _entityType;

public:
    Animation();
    Animation(std::shared_ptr<MeshRenderer> meshRenderer, float frameDuration, bool loop);

    nlohmann::json Serialize() override;
    void Deserialize(const nlohmann::json &jsonData) override;
    void Initiate() override;

    void AddFrame(std::shared_ptr<Model> model);
    std::shared_ptr<Model> GetCurrentFrame();
    void Update();
    void Reset();
    void InitFrames();

    //void addToInspector(ImguiMain *imguiMain) override;

};