//
// Created by TheWojwymX on 14.04.2024.
//

#include "Animation.h"

Animation::Animation(std::shared_ptr<MeshRenderer> meshRenderer, float frameDuration, bool loop)
        : _meshRenderer(meshRenderer), _frameDuration(frameDuration), _currentTime(0.0f), _loop(loop) {}

void Animation::AddFrame(std::shared_ptr<Model> model)
{
    _frames.push_back(model);
}

std::shared_ptr<Model> Animation::GetCurrentFrame()
{
    if (_frames.empty()) {
        return nullptr;
    }

    int frameIndex = static_cast<int>(_currentTime / _frameDuration) % _frames.size();
    return _frames[frameIndex];
}

void Animation::Update(float deltaTime) {
    _currentTime += deltaTime;

    if (_loop)
    {
        while (_currentTime >= _frameDuration * _frames.size())
        {
            _currentTime -= _frameDuration * _frames.size();
        }
    }
    else
    {
        _currentTime = std::min(_currentTime, _frameDuration * _frames.size() - 0.01f);
    }
}

void Animation::Reset()
{
    _currentTime = 0.0f;
}

void Animation::Draw(float dt)
{
    Update(dt);
    _meshRenderer->setModel(GetCurrentFrame().get());
    _ownerTransform->SetPosition(glm::vec3(10,110,10));
    _meshRenderer->_ownerTransform->SetPosition(glm::vec3(10, 110, 10));
    _meshRenderer->Render(Transform::Origin());
}

