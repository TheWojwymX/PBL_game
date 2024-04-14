#ifndef OPENGLGP_ANIMATOR_H
#define OPENGLGP_ANIMATOR_H

#include <iostream>
#include <vector>
#include "model.h"
#include "Core/Component.h"

class Animator: public Component {
private:
    std::string _name;
    std::vector<std::shared_ptr<Model>> _frames;
    float _frameDuration; // Duration of each frame in seconds
    float _currentTime;   // Current time in the animation
    bool _loop;           // Whether the animation should loop

public:
    Animator(const std::string& name, float frameDuration, bool loop);
    void AddFrame(std::shared_ptr<Model> model);
    std::shared_ptr<Model> GetCurrentFrame();
    void Update(float deltaTime);
    void Reset();
    bool IsFinished();
    void addToInspector(ImguiMain *imguiMain) override;




};


#endif //OPENGLGP_ANIMATOR_H
