//
// Created by Jacek on 11.04.2024.
//

#include "AudioManager.h"
#include <iostream>

AudioManager &AudioManager::getInstance() {
    static AudioManager instance;
    return instance;
}

void AudioManager::Init() {

    // Setup miniaudio
    _result = ma_engine_init(NULL, &_engine);
    if (_result != MA_SUCCESS) {
        std::cout << ("Failed to initialize audio engine.") << std::endl;
    }

}

void AudioManager::Update() {

}

void AudioManager::Cleanup() {
    ma_engine_uninit(&_engine);
}

void AudioManager::PlayBackgroundMusic() {
    ma_engine_play_sound(&_engine, "../../res/Sounds/ksu.mp3", NULL);
}

void AudioManager::PlaySound() {
    ma_engine_play_sound(&_engine, "../../res/Sounds/concrete-footsteps-6752.mp3", NULL);
}
