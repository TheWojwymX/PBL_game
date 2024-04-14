//
// Created by TheWojwymX on 14.04.2024.
//

#include "Animator.h"

Animator::Animator(const std::string& name, float frameDuration, bool loop)
        : _name(name), _frameDuration(frameDuration), _currentTime(0.0f), _loop(loop) {}

void Animator::AddFrame(std::shared_ptr<Model> model) {
    _frames.push_back(model);
}

std::shared_ptr<Model> Animator::GetCurrentFrame() {
    if (_frames.empty()) {
        return nullptr;
    }

    int frameIndex = static_cast<int>(_currentTime / _frameDuration) % _frames.size();
    return _frames[frameIndex];
}

void Animator::Update(float deltaTime) {
    _currentTime += deltaTime;

    if (_loop) {
        while (_currentTime >= _frameDuration * _frames.size()) {
            _currentTime -= _frameDuration * _frames.size();
        }
    } else {
        _currentTime = std::min(_currentTime, _frameDuration * _frames.size() - 0.01f);
    }
}

void Animator::Reset()
{
    _currentTime = 0.0f;
}

bool Animator::IsFinished()
{
    return !_loop && _currentTime >= _frameDuration * _frames.size();
}
