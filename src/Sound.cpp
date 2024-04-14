//
// Created by Jacek on 11.04.2024.
//

#include "Sound.h"

Sound::Sound(const std::string &name, const std::string &path, int id, SoundType soundType) : _name(name), _path(path), _id(id), _soundType(soundType) {

    _result = ma_sound_init_from_file(&AUDIOMANAGER._engine, _path.c_str(), 0, NULL, NULL, &_sound);
    if (_result != MA_SUCCESS) {
        std::cout << "Can't load sound file: " << _name << " at path: " << _path << std::endl;
    }
    else{
        if(_soundType == SoundType::MUSIC){
            ChangeVolume(AUDIOMANAGER._musicVolume);
        }
        else if(_soundType == SoundType::SFX){
            ChangeVolume(AUDIOMANAGER._sfxVolume);
        }
    }

}

void Sound::PlaySound() {
    ma_sound_start(&_sound);
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
