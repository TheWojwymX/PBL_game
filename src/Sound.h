//
// Created by Jacek on 11.04.2024.
//

#ifndef OPENGLGP_SOUND_H
#define OPENGLGP_SOUND_H

#include "../thirdparty/miniaudio.h"
#include <string>
#include "Managers/AudioEngineManager.h"
#include <iostream>
#include "../thirdparty/nlohmann/json.hpp"

class Node;

enum class SoundType {
    MUSIC,
    SFX
};

class Sound {
public:
    std::string _name;
    std::string _path;
    int _id;

    ma_result _result;
    ma_sound _sound;

    SoundType _soundType;

    Sound(const std::string &name, const std::string &path, int id, SoundType soundType);
    ~Sound();

    nlohmann::json Serialize();

    void PlaySound(std::shared_ptr<Node> soundSourceNode, float adjVolume = 1.0f);
    void StopSound();

    void ChangeVolume(float volume);

    void SetLooping(bool looping);

    void PlaySoundSim(std::shared_ptr<Node> soundSourceNode, float adjVolume = 1.0f);

    std::shared_ptr<Node> _playerNode = nullptr;

    float CalculateVolumeToPlayerDistance(std::shared_ptr<Node> soundSourceNode);

    float _maxSoundDistance = 20;
};


#endif //OPENGLGP_SOUND_H
