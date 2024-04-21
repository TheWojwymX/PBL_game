//
// Created by TheWojwymX on 14.04.2024.
//

#include "Animation.h"

Animation::Animation(std::shared_ptr<MeshRenderer> meshRenderer, float frameDuration, bool loop)
        : _meshRenderer(meshRenderer), _frameDuration(frameDuration), _currentTime(0.0f), _loop(loop) {}

Animation::Animation() {
    _type = ComponentType::ANIMATION;
}

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
    _meshRenderer->_model = GetCurrentFrame();
    _meshRenderer->Render(Transform::Origin());
}

nlohmann::json Animation::Serialize() {
    nlohmann::json data = Component::Serialize();

    data["meshRendererRefID"] = _meshRenderer->_id;
    data["frameDuration"] = _frameDuration;
    data["isLoop"] = _loop;

    return data;
}

void Animation::Deserialize(const nlohmann::json &jsonData) {

    if (jsonData.contains("meshRendererRefID")) {
        _meshRendererID = jsonData["meshRendererRefID"].get<int>();
    }

    if (jsonData.contains("frameDuration")) {
        _frameDuration = jsonData["frameDuration"].get<float>();
    }

    if (jsonData.contains("isLoop")) {
        _loop = jsonData["isLoop"].get<bool>();
    }

    Component::Deserialize(jsonData);
}

void Animation::initiate() {
    _meshRenderer = COMPONENTSMANAGER.GetComponentByID<MeshRenderer>(_meshRendererID);
    Component::initiate();
}

