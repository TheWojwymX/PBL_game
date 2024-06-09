//
// Created by Jacek on 11.04.2024.
//

#include <thread>
#include "Sound.h"

Sound::Sound(const std::string &name, const std::string &path, int id, SoundType soundType) : _name(name), _path(path), _id(id), _soundType(soundType) {

    _result = ma_sound_init_from_file(&AUDIOENGINEMANAGER._engine, _path.c_str(), 0, NULL, NULL, &_sound);
    if (_result != MA_SUCCESS) {
        std::cout << "Can't load sound file: " << _name << " at path: " << _path << std::endl;
    }
    else{
        if(_soundType == SoundType::MUSIC){
            ChangeVolume(AUDIOENGINEMANAGER._musicVolume);
        }
        else if(_soundType == SoundType::SFX){
            ChangeVolume(AUDIOENGINEMANAGER._sfxVolume);
        }
    }
}

Sound::~Sound() {
    ma_sound_uninit(&_sound);
    std::cout << "Sound uninitialized: " << _name << std::endl;
}

void Sound::PlaySound() {
    if (ma_sound_start(&_sound) != MA_SUCCESS) {
        std::cerr << "Failed to start sound: " << _name << std::endl;
    } else {
        //std::cout << "Playing sound: " << _name << std::endl;
    }
}

void Sound::PlaySoundSim() {
    ma_sound* sound = new ma_sound; // Dynamically allocate memory for the sound instance

    //std::cout << sound->pDataSource << std::endl;

    auto result = ma_sound_init_from_file(&AUDIOENGINEMANAGER._engine, _path.c_str(), 0, nullptr, nullptr, sound);
    if (result != MA_SUCCESS) {
        std::cerr << "Can't load sound file: " << _name << " at path: " << _path << std::endl;
        delete sound; // Clean up the allocated memory
        return;
    }

    // Set the volume based on the sound type
    if (_soundType == SoundType::MUSIC) {
        ma_sound_set_volume(sound, AUDIOENGINEMANAGER._musicVolume);
    } else if (_soundType == SoundType::SFX) {
        ma_sound_set_volume(sound, AUDIOENGINEMANAGER._sfxVolume);
    }

    result = ma_sound_start(sound);

    if (result != MA_SUCCESS) {
        std::cerr << "Failed to start sound. Error: " << result << std::endl;
        ma_sound_uninit(sound);
        delete sound;
        return;
    }

    AUDIOENGINEMANAGER._activeSounds.push_back(sound);

}

nlohmann::json Sound::Serialize() {
    nlohmann::json soundJson;

    soundJson["SoundName"] = _name;
    soundJson["Path"] = _path;
    soundJson["Type"] = _soundType;
    soundJson["SoundID"] = _id;

    return soundJson;
}

void Sound::ChangeVolume(float volume) {
    ma_sound_set_volume(&_sound, volume);
}

void Sound::SetLooping(bool looping) {
    ma_sound_set_looping(&_sound, looping);
}


/*
// Fade in over 1 second.
ma_sound_set_fade_in_milliseconds(&sound, 0, 1, 1000);

// ... sometime later ...

// Fade out over 1 second, starting from the current volume.
ma_sound_set_fade_in_milliseconds(&sound, -1, 0, 1000);*/
