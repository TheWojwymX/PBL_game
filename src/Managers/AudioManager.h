//
// Created by Jacek on 11.04.2024.
//

#define AUDIOMANAGER AudioManager::getInstance()

#ifndef OPENGLGP_AUDIOMANAGER_H
#define OPENGLGP_AUDIOMANAGER_H

#include "../thirdparty/miniaudio.h"

class AudioManager {

public:

    static AudioManager &getInstance();

    ~AudioManager() = default;

    AudioManager() = default;

    AudioManager(const AudioManager &) = delete;

    AudioManager &operator=(const AudioManager &) = delete;

    void Init();

    void Update();

    void Cleanup();

    void PlayBackgroundMusic();

    void PlaySound();

    ma_result _result;
    ma_engine _engine;

    float _musicVolume = 1;
    float _sfxVolume = 1;
};


#endif //OPENGLGP_AUDIOMANAGER_H
