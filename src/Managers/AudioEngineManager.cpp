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

void AudioEngineManager::Update() {

}

void AudioEngineManager::Cleanup() {
    ma_engine_uninit(&_engine);
}