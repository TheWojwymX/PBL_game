#pragma once

#include <iostream>
#include <vector>
#include "model.h"
#include "Core/Component.h"
#include "MeshRenderer.h"

class Animation: public Component {
private:
    std::vector<std::shared_ptr<Model>> _frames;
    float _frameDuration; // Duration of each frame in seconds
    float _currentTime;   // Current time in the animation
    bool _loop;           // Whether the animation should loop
    MeshRenderer _meshRenderer;

public:
    Animation(MeshRenderer meshRenderer, float frameDuration, bool loop);
    void AddFrame(std::shared_ptr<Model> model);
    std::shared_ptr<Model> GetCurrentFrame();
    void Update(float deltaTime);
    void Reset();
    void Draw(float dt);
    //void addToInspector(ImguiMain *imguiMain) override;

};


