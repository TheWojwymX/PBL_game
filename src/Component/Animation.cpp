#pragma once

#include <random>
#include "Animation.h"
#include "Core/Time.h"
#include "Core/Node.h"

Animation::Animation(std::shared_ptr<MeshRenderer> meshRenderer, float frameDuration, bool loop, state enemyState)
        : _meshRenderer(meshRenderer), _frameDuration(frameDuration), _currentTime(0.0f), _loop(loop), _enemyState(enemyState), _randomGen(10,0.0f,0.5f){}

Animation::Animation(): _randomGen(10, 0.0f, 0.25f)
{
    _type = ComponentType::ANIMATION;
}

std::shared_ptr<Model> Animation::GetCurrentFrame()
{
    switch(_enemyState)
    {
        case SPAWN:
        {
            if (_spawnFrames.empty())
            {
                return nullptr;
            }
            int frameIndex = static_cast<int>(_currentTime / _frameDuration) % _spawnFrames.size();
            return _spawnFrames[frameIndex];
        }

        case WALK:
        {
            if (_walkFrames.empty())
            {
                return nullptr;
            }
            int frameIndex = static_cast<int>(_currentTime / _frameDuration) % _walkFrames.size();
            return _walkFrames[frameIndex];
        }

        case ATTACK:
        {
            if (_attackFrames.empty()) {
                return nullptr;
            }

            int frameIndex = static_cast<int>(_currentTime / _frameDuration) % _attackFrames.size();
            return _attackFrames[frameIndex];
        }

        case DEAD:
        {
            if (_deadFrames.empty())
            {
                return nullptr;
            }

            int frameIndex;

            if(IsAnimationFinished())
            {
                frameIndex = static_cast<int>(_deadFrames.size()-1);
            }
            else
            {
                frameIndex = static_cast<int>(_currentTime / _frameDuration) % _deadFrames.size();
            }

            return _deadFrames[frameIndex];
        }
    }

}

void Animation::Update() {
    if (GAMEMANAGER._paused) return;

    float deltaTime = TIME.GetDeltaTime();
    _currentTime += deltaTime;

    if (_enemyState == SPAWN && !IsAnimationFinished())
    {
        _currentTime = std::min(_currentTime, _frameDuration * _spawnFrames.size() - 0.01f);
    }
    else if (_enemyState == SPAWN && IsAnimationFinished())
    {
        _enemyState = WALK;
        _currentTime = _randomGen.GetRandomFloat();
    }

    if (_enemyState == DEAD && IsAnimationFinished())
    {
        if (_enemyType == 2)
        {
            _toDelete = true;
            return;
        }

        if(_meshRenderer->GetOwnerNode()->GetTransform()->GetPosition().y > GAMEMANAGER._groundLevel - 2)
        {
            _meshRenderer->GetOwnerNode()->GetTransform()->SetPosition(_meshRenderer->GetOwnerNode()->GetTransform()->GetPosition().y - TIME.GetDeltaTime(), 1);
        }
        else
        {
            _toDelete = true;
        }
    }

    if (_loop)
    {
        switch (_enemyState)
        {
            case WALK:
            {
                if (_currentTime >= _frameDuration * _walkFrames.size())
                {
                    _currentTime = 0.0f;
                }
                break;
            }

            case ATTACK:
            {
                if (_currentTime >= _frameDuration * _attackFrames.size())
                {
                    _currentTime = 0.0f;
                }
                break;
            }
        }
    }
    else
    {
        switch (_enemyState)
        {
            case WALK:
            {
                _currentTime = std::min(_currentTime, _frameDuration * _walkFrames.size() - 0.01f);
                break;
            }

            case ATTACK:
            {
                _currentTime = std::min(_currentTime, _frameDuration * _attackFrames.size() - 0.01f);
                break;
            }
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
    data["entityType"] = _enemyType;
    data["meshRendererRefID"] = _meshRenderer->_id;
    data["frameDuration"] = _frameDuration;
    data["isLoop"] = _loop;

    return data;
}

void Animation::Deserialize(const nlohmann::json &jsonData) {
    _walkFrames.clear();
    _attackFrames.clear();

    if (jsonData.contains("entityType")) {
        _enemyType = jsonData["entityType"].get<int>();
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

    Component::Deserialize(jsonData);
}

void Animation::Initiate() {
    _meshRenderer = COMPONENTSMANAGER.GetComponentByID<MeshRenderer>(_meshRendererID);
    Component::Initiate();
}

void Animation::InitFrames(std::vector <std::vector<std::shared_ptr<Model>>> frames)
{
    _spawnFrames = frames[0];
    _walkFrames = frames[1];
    _attackFrames = frames[2];
    _deadFrames = frames[3];
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

void Animation::InitComponent(int type, std::vector<std::vector<std::shared_ptr<Model>>> frames)
{
    _currentTime = 0.0f;
    _enemyType = type;
    _enemyState = SPAWN;
    _loop = true;
    _frameDuration = 0.15;
    InitFrames(frames);
}

bool Animation::IsAnimationFinished()
{
    switch (_enemyState) {
        case SPAWN:
            return _currentTime >= _frameDuration * _spawnFrames.size();
        case WALK:
            return _currentTime >= _frameDuration * _walkFrames.size();
        case ATTACK:
            return _currentTime >= _frameDuration * _attackFrames.size();
        case DEAD:
            return _currentTime >= _frameDuration * _deadFrames.size();
        default:
            return false;
    }
}

void Animation::setEntityType(int type)
{
    _enemyType = type;
}
