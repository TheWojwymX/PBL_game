//
// Created by Jacek on 11.04.2024.
//

#ifndef OPENGLGP_SOUND_H
#define OPENGLGP_SOUND_H

#include "../thirdparty/miniaudio.h"
#include <string>
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

    Sound(const std::string &name, const std::string &path, int id, SoundType soundType, float volumeMultiplier);
    ~Sound();

    nlohmann::json Serialize();

    void PlaySound(std::shared_ptr<Node> soundSourceNode, float adjVolume = 1.0f);
    void StopSound();

    void ChangeVolume(float volume);

    void SetLooping(bool looping);

    void PlaySoundSim(std::shared_ptr<Node> soundSourceNode, float adjVolume = 1.0f);

    std::shared_ptr<Node> _playerNode = nullptr;

    float CalculateVolumeToPlayerDistance(std::shared_ptr<Node> soundSourceNode);

    float _maxSoundDistance = 30;

    bool _isFadingAway = false;
    bool _isRisingUp = false;

    void Update();


    void FadeAway(float time);
    void RiseUp(float time, std::shared_ptr<Node> sourceNode);

    void SetVolume(float volume);

    void StopFadingAway();
private:
    float _timeToFadeAway;
    float _timeToRiseUp;
    float _timer = 0.0f;

    float _constStartingVolumeMultiplier;
    std::shared_ptr<float> _typeMultiplier;
    float _volume = 1.0f;
};


#endif //OPENGLGP_SOUND_H
