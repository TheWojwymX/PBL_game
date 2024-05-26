#pragma once

#include "Animation.h"
#include "Core/Time.h"

Animation::Animation(std::shared_ptr<MeshRenderer> meshRenderer, float frameDuration, bool loop, bool isWalking)
        : _meshRenderer(meshRenderer), _frameDuration(frameDuration), _currentTime(0.0f), _loop(loop), _isWalking(isWalking) {}

Animation::Animation()
{
    _type = ComponentType::ANIMATION;
}

void Animation::AddWalkFrame(std::shared_ptr<Model> model)
{
    _walkFrames.push_back(model);
}

void Animation::AddAttackFrame(std::shared_ptr<Model> model)
{
    _attackFrames.push_back(model);
}

void Animation::setIsWalking(bool state)
{
    _isWalking = state;
}

std::shared_ptr<Model> Animation::GetCurrentFrame()
{
    if(_isWalking)
    {
        if (_walkFrames.empty()) {
            return nullptr;
        }

        int frameIndex = static_cast<int>(_currentTime / _frameDuration) % _walkFrames.size();
        return _walkFrames[frameIndex];
    }
    else
    {
        if (_attackFrames.empty()) {
            return nullptr;
        }

        int frameIndex = static_cast<int>(_currentTime / _frameDuration) % _attackFrames.size();
        return _attackFrames[frameIndex];

    }
}

void Animation::Update() {
    float deltaTime = TIME.GetDeltaTime();
    _currentTime += deltaTime;

    if (_loop)
    {
        if(_isWalking)
        {
            while (_currentTime >= _frameDuration * _walkFrames.size())
            {
                _currentTime -= _frameDuration * _walkFrames.size();
            }
        }
        else
        {
            while (_currentTime >= _frameDuration * _attackFrames.size())
            {
                _currentTime -= _frameDuration * _attackFrames.size();
            }
        }
    }
    else
    {
        if(_isWalking)
        {
            _currentTime = std::min(_currentTime, _frameDuration * _walkFrames.size() - 0.01f);
        }
        else
        {
            _currentTime = std::min(_currentTime, _frameDuration * _attackFrames.size() - 0.01f);
        }
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
    _walkFrames.clear();
    _attackFrames.clear();

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
        for(int i = 0; i < 6; i++)
        {
            AddWalkFrame(RESOURCEMANAGER.GetModelByName("AntWalk" + to_string(i)));
        }
        for(int i = 0; i < 4; i++)
        {
            AddAttackFrame(RESOURCEMANAGER.GetModelByName("AntAttack" + to_string(i)));
        }
    }
}

void Animation::setEntityType(int type)
{
    _entityType = type;
}

void Animation::setCurrentTime(float time)
{
    _currentTime = time;
}

void Animation::setMeshRenderer(const shared_ptr<MeshRenderer> &meshRenderer) {
    _meshRenderer = meshRenderer;
}

void Animation::setFrameDuration(float frameDuration) {
    _frameDuration = frameDuration;
}

void Animation::setLoop(bool loop) {
    _loop = loop;
}

void Animation::setMeshRendererId(int meshRendererId) {
    _meshRendererID = meshRendererId;
}

void Animation::InitComponent(int type)
{
    _currentTime = 0;
    _entityType = type;
    _isWalking = true;
    _loop = true;
    _frameDuration = 0.2;
    InitFrames();
}
