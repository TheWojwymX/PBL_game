//
// Created by Jacek on 11.04.2024.
//

#include "AudioEngineManager.h"
#include <iostream>

AudioEngineManager &AudioEngineManager::getInstance() {
    static AudioEngineManager instance;
    return instance;
}

void AudioEngineManager::Init() {

    // Setup miniaudio
    _result = ma_engine_init(NULL, &_engine);
    if (_result != MA_SUCCESS) {
        std::cout << ("Failed to initialize audio engine.") << std::endl;
    }

}

void AudioEngineManager::Update()
{
    CleanupFinishedSounds();
}

void AudioEngineManager::Cleanup()
{
    for (auto sound : _activeSounds) {
        ma_sound_uninit(sound);
        delete sound;
    }
    _activeSounds.clear();
    ma_engine_uninit(&_engine);
    std::cout << "Audio engine cleaned up." << std::endl;
}

void AudioEngineManager::CleanupFinishedSounds()
{
    auto it = _activeSounds.begin();
    while (it != _activeSounds.end()) {
        if (!ma_sound_is_playing(*it)) {
            ma_sound_uninit(*it);
            delete *it;
            it = _activeSounds.erase(it);
            std::cout << "Finished sound cleaned up." << std::endl;
        } else {
            ++it;
        }
    }
}