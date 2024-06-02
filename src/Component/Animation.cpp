#pragma once

#include <random>
#include "Animation.h"
#include "Core/Time.h"

Animation::Animation(std::shared_ptr<MeshRenderer> meshRenderer, float frameDuration, bool loop, state enemyState)
        : _meshRenderer(meshRenderer), _frameDuration(frameDuration), _currentTime(0.0f), _loop(loop), _enemyState(enemyState) {}

Animation::Animation()
{
    _type = ComponentType::ANIMATION;
}

float generateRandomFloat(std::mt19937& gen, std::uniform_real_distribution<float>& dis) {
    return dis(gen);
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
            int frameIndex = static_cast<int>(_currentTime / _frameDuration) % _deadFrames.size();
            return _deadFrames[frameIndex];
        }
    }

}

void Animation::Update() {
    float deltaTime = TIME.GetDeltaTime();
    _currentTime += deltaTime;

    if (_enemyState == SPAWN && !IsAnimationFinished())
    {
        _currentTime = std::min(_currentTime, _frameDuration * _spawnFrames.size() - 0.01f);
    }
    else if (_enemyState == SPAWN && IsAnimationFinished())
    {
        _enemyState = WALK;
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(0.0f, 0.5f);
        _currentTime = generateRandomFloat(gen, dis);
    }

    if (_loop)
    {
        switch (_enemyState)
        {
            case WALK:
            {
                while (_currentTime >= _frameDuration * _walkFrames.size())
                {
                    _currentTime -= _frameDuration * _walkFrames.size();
                }
                break;
            }

            case ATTACK:
            {
                while (_currentTime >= _frameDuration * _attackFrames.size())
                {
                    _currentTime -= _frameDuration * _attackFrames.size();
                }
                break;
            }

            case DEAD:
            {
                while (_currentTime >= _frameDuration * _deadFrames.size())
                {
                    _currentTime -= _frameDuration * _deadFrames.size();
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

            case DEAD:
            {
                _currentTime = std::min(_currentTime, _frameDuration * _deadFrames.size() - 0.01f);
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
        _spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("AntSpawn0"));
        _spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("AntSpawn1"));
        _spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("AntSpawn2"));
        _spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("AntSpawn3"));
        _spawnFrames.push_back(RESOURCEMANAGER.GetModelByName("AntSpawn4"));

        _walkFrames.push_back(RESOURCEMANAGER.GetModelByName("AntWalk0"));
        _walkFrames.push_back(RESOURCEMANAGER.GetModelByName("AntWalk1"));
        _walkFrames.push_back(RESOURCEMANAGER.GetModelByName("AntWalk2"));
        _walkFrames.push_back(RESOURCEMANAGER.GetModelByName("AntWalk3"));
        _walkFrames.push_back(RESOURCEMANAGER.GetModelByName("AntWalk4"));
        _walkFrames.push_back(RESOURCEMANAGER.GetModelByName("AntWalk5"));

        _attackFrames.push_back(RESOURCEMANAGER.GetModelByName("AntAttack0"));
        _attackFrames.push_back(RESOURCEMANAGER.GetModelByName("AntAttack1"));
        _attackFrames.push_back(RESOURCEMANAGER.GetModelByName("AntAttack2"));
        _attackFrames.push_back(RESOURCEMANAGER.GetModelByName("AntAttack3"));

        _deadFrames.push_back(RESOURCEMANAGER.GetModelByName("AntDead0"));
        _deadFrames.push_back(RESOURCEMANAGER.GetModelByName("AntDead1"));
        _deadFrames.push_back(RESOURCEMANAGER.GetModelByName("AntDead2"));
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
    _currentTime = 0.0f;
    _entityType = type;
    _enemyState = SPAWN;
    _loop = true;
    _frameDuration = 0.2;
    InitFrames();
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
