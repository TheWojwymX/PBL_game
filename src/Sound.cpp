//
// Created by Jacek on 11.04.2024.
//

#include "Sound.h"

Sound::Sound(const std::string &soundName, const std::string &path) : _name(soundName), _path(path) {

    _result = ma_sound_init_from_file(&AUDIOMANAGER._engine, "ksu.mp3", 0, NULL, NULL, &_sound);
    if (_result != MA_SUCCESS) {
        std::cout << "Can't load sound file" << std::endl;
    }

}

void Sound::PlaySound() {
    ma_sound_start(&_sound);
}

nlohmann::json Sound::Serialize() {
    nlohmann::json soundJson;

    soundJson["SoundName"] = _name;
    soundJson["Path"] = _path;

    return soundJson;
}
