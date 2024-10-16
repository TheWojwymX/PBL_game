//
// Created by Jacek on 11.04.2024.
//

#define AUDIOMANAGER AudioManager::getInstance()

#ifndef OPENGLGP_AUDIOMANAGER_H
#define OPENGLGP_AUDIOMANAGER_H

#include <vector>
#include <memory>
#include "../thirdparty/miniaudio.h"
#include "Sound.h"

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

    ma_result _result;
    ma_engine _engine;

    float _musicVolume = 0.5f;
    float _sfxVolume = 0.5f;

    std::vector<ma_sound*> _activeSounds;
    void CleanupFinishedSounds();

    std::vector<Sound*> _sounds;

    void Reset();

    void RefreshSoundsVolumes();

    void ChangeMusicVolume(int value);

    void ChangeSFXVolume(int value);
};


#endif //OPENGLGP_AUDIOMANAGER_H
