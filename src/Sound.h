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

    nlohmann::json Serialize();

    void PlaySound();

    void ChangeVolume(float volume);

    void SetLooping(bool looping);

};


#endif //OPENGLGP_SOUND_H
