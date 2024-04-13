//
// Created by Jacek on 11.04.2024.
//

#ifndef OPENGLGP_SOUND_H
#define OPENGLGP_SOUND_H

#include "../thirdparty/miniaudio.h"
#include <string>
#include "Managers/AudioManager.h"
#include <iostream>
#include "../thirdparty/nlohmann/json.hpp"

class Sound {
public:
    std::string _name;
    std::string _path;

    ma_result _result;
    ma_sound _sound;

    Sound(const std::string &soundName, const std::string &path);

    nlohmann::json Serialize();

    void PlaySound();

};


#endif //OPENGLGP_SOUND_H
