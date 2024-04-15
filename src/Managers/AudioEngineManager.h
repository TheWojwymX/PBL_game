//
// Created by Jacek on 11.04.2024.
//

#define AUDIOENGINEMANAGER AudioEngineManager::getInstance()

#ifndef OPENGLGP_AUDIOMANAGER_H
#define OPENGLGP_AUDIOMANAGER_H

#include "../thirdparty/miniaudio.h"

class AudioEngineManager {

public:

    static AudioEngineManager &getInstance();

    ~AudioEngineManager() = default;

    AudioEngineManager() = default;

    AudioEngineManager(const AudioEngineManager &) = delete;

    AudioEngineManager &operator=(const AudioEngineManager &) = delete;

    void Init();

    void Update();

    void Cleanup();

    ma_result _result;
    ma_engine _engine;

    float _musicVolume = 1;
    float _sfxVolume = 1;
};


#endif //OPENGLGP_AUDIOMANAGER_H
