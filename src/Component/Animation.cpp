#pragma once

#include "Animation.h"
#include "Core/Time.h"

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

void Animation::Update() {
    float deltaTime = TIME.GetDeltaTime();
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

    _meshRenderer->_model = GetCurrentFrame();
    _meshRenderer->Render(Transform::Origin());
}

void Animation::Reset()
{
    _currentTime = 0.0f;
}

nlohmann::json Animation::Serialize() {
    nlohmann::json data = Component::Serialize();
    data["entityType"] = _entityType;
    data["meshRendererRefID"] = _meshRenderer->_id;
    data["frameDuration"] = _frameDuration;
    data["isLoop"] = _loop;

    return data;
}

void Animation::Deserialize(const nlohmann::json &jsonData) {
    _frames.clear();

    if (jsonData.contains("entityType")) {
        _entityType = jsonData["entityType"].get<int>();
    }

    if (jsonData.contains("meshRendererRefID")) {
        _meshRendererID = jsonData["meshRendererRefID"].get<int>();
    }

    if (jsonData.contains("frameDuration")) {
        _frameDuration = jsonData["frameDuration"].get<float>();
    }

    if (jsonData.contains("isLoop")) {
        _loop = jsonData["isLoop"].get<bool>();
    }

    InitFrames();

    Component::Deserialize(jsonData);
}

void Animation::Initiate() {
    _meshRenderer = COMPONENTSMANAGER.GetComponentByID<MeshRenderer>(_meshRendererID);
    Component::Initiate();
}

void Animation::InitFrames()
{
    if (_entityType == 1)
    {
        AddFrame(RESOURCEMANAGER.GetModelByName("antModel"));
//        AddFrame(RESOURCEMANAGER.GetModelByName("AntWalk1"));
//        AddFrame(RESOURCEMANAGER.GetModelByName("AntWalk2"));
//        AddFrame(RESOURCEMANAGER.GetModelByName("AntWalk3"));
    }

}