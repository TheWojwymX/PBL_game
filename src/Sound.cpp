//
// Created by Jacek on 11.04.2024.
//

#include <thread>
#include "Sound.h"
#include "Core/Node.h"

Sound::Sound(const std::string &name, const std::string &path, int id, SoundType soundType)
        : _name(name), _path(path), _id(id), _soundType(soundType) {

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

Sound::~Sound()
{
    std::cout << "Sound uninitialized: " << _name << std::endl;
}

void Sound::PlaySound(std::shared_ptr<Node> soundSourceNode) {
    if(_playerNode == nullptr){
        _playerNode = NODESMANAGER.getNodeByName("player");
    }

    float volume = CalculateVolumeToPlayerDistance(soundSourceNode);
    ChangeVolume(volume);

    if (ma_sound_start(&_sound) != MA_SUCCESS) {
        std::cerr << "Failed to start sound: " << _name << std::endl;
    } else {
        //std::cout << "Playing sound: " << _name << std::endl;
    }
}

void Sound::PlaySoundSim(std::shared_ptr<Node> soundSourceNode) {

    if(_playerNode == nullptr){
        _playerNode = NODESMANAGER.getNodeByName("player");
    }

    float volume = CalculateVolumeToPlayerDistance(soundSourceNode);

    ma_sound* sound = new ma_sound; // Dynamically allocate memory for the sound instance

    //std::cout << sound->pDataSource << std::endl;

    auto result = ma_sound_init_from_file(&AUDIOENGINEMANAGER._engine, _path.c_str(), 0, nullptr, nullptr, sound);
    if (result != MA_SUCCESS) {
        std::cerr << "Can't load sound file: " << _name << " at path: " << _path << std::endl;
        delete sound; // Clean up the allocated memory
        return;
    }

    // Set the volume based on the sound type
    ma_sound_set_volume(sound, volume);

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

float Sound::CalculateVolumeToPlayerDistance(std::shared_ptr<Node> soundSourceNode) {
    float distance = glm::distance(_playerNode->GetTransform()->GetPosition(), soundSourceNode->GetTransform()->GetPosition());

    if (distance <= 0) {
        if (_soundType == SoundType::MUSIC) {
            return AUDIOENGINEMANAGER._musicVolume;
        } else if (_soundType == SoundType::SFX) {
            return AUDIOENGINEMANAGER._sfxVolume;
        }
    } else if (distance < _maxSoundDistance) {
        float normalizedDistance = distance / _maxSoundDistance;
        float logDistance = log(normalizedDistance + 1); // log(1) = 0, log(x) where x -> 0 is negative, thus adding 1 to avoid negative values

        if (_soundType == SoundType::MUSIC) {
            float maxVolume = AUDIOENGINEMANAGER._musicVolume;
            return maxVolume - maxVolume * logDistance;
        } else if (_soundType == SoundType::SFX) {
            float maxVolume = AUDIOENGINEMANAGER._sfxVolume;
            std::cout << maxVolume - maxVolume * logDistance << std::endl;
            return maxVolume - maxVolume * logDistance;
        }
    } else {
        return 0;
    }
}
