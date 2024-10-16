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

void AudioManager::Update()
{

    for(Sound* sound : _sounds){
        sound->Update();
    }

    CleanupFinishedSounds();
}

void AudioManager::Cleanup()
{
    for (auto sound : _activeSounds) {
        ma_sound_uninit(sound);
        delete sound;
    }
    _activeSounds.clear();
    ma_engine_uninit(&_engine);
    std::cout << "Audio engine cleaned up." << std::endl;
}

void AudioManager::CleanupFinishedSounds()
{
    auto it = _activeSounds.begin();
    while (it != _activeSounds.end()) {
        if (!ma_sound_is_playing(*it)) {
            ma_sound_uninit(*it);
            delete *it;
            it = _activeSounds.erase(it);
            //std::cout << "Finished sound cleaned up." << std::endl;
        } else {
            ++it;
        }
    }
}

void AudioManager::Reset() {
    for (auto sound : _sounds) {
        sound->Reset();
    }
}

void AudioManager::RefreshSoundsVolumes(){
    for (auto sound : _sounds) {
        sound->RefreshVolume();
    }
}

void AudioManager::ChangeMusicVolume(int value) {
    float currentVolumePercentage = _musicVolume * 100.0f;
    float newVolumePercentage = currentVolumePercentage + value;
    if (newVolumePercentage < 0) {
        newVolumePercentage = 0;
    } else if (newVolumePercentage > 100) {
        newVolumePercentage = 100;
    }
    _musicVolume = newVolumePercentage / 100.0f;
    RefreshSoundsVolumes();
}


void AudioManager::ChangeSFXVolume(int value){
    float currentVolumePercentage = _sfxVolume * 100.0f;
    float newVolumePercentage = currentVolumePercentage + value;
    if (newVolumePercentage < 0) {
        newVolumePercentage = 0;
    } else if (newVolumePercentage > 100) {
        newVolumePercentage = 100;
    }
    _sfxVolume = newVolumePercentage / 100.0f;
    RefreshSoundsVolumes();
}
