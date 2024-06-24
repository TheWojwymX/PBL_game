//
// Created by Jacek on 11.04.2024.
//

#include <thread>
#include "Sound.h"
#include "Core/Node.h"
#include "Managers/AudioManager.h"

Sound::Sound(const std::string &name, const std::string &path, int id, SoundType soundType, float volumeMultiplier)
        : _name(name), _path(path), _id(id), _soundType(soundType), _constStartingVolumeMultiplier(volumeMultiplier) {

    _result = ma_sound_init_from_file(&AUDIOMANAGER._engine, _path.c_str(), 0, NULL, NULL, &_sound);
    if (_result != MA_SUCCESS) {
        std::cout << "Can't load sound file: " << _name << " at path: " << _path << std::endl;
    }
    else{
        if(_soundType == SoundType::MUSIC){
            _typeMultiplier = &AUDIOMANAGER._musicVolume;
        }
        else if(_soundType == SoundType::SFX){
            _typeMultiplier = &AUDIOMANAGER._sfxVolume;
        }
        _volume = 1.0f;
        AUDIOMANAGER._sounds.push_back(this);
    }
}

Sound::~Sound()
{
    //std::cout << "Sound uninitialized: " << _name << std::endl;
}

void Sound::Update() {

    if(_isFadingAway){
        if(_timer < _timeToFadeAway){
            _timer += TIME.GetDeltaTime();
            float t = glm::clamp(_timer / _timeToFadeAway, 0.0f, 1.0f);
            float volume = glm::mix(_volume, 0.0f, t);
            ChangeVolume(volume);
        }else{
            _isFadingAway = false;
            _timer = 0.0f;
            StopSound();
        }
    }

    else if(_isRisingUp){
        if(_timer < _timeToRiseUp){
            _timer += TIME.GetDeltaTime();
            float t = glm::clamp(_timer / _timeToRiseUp, 0.0f, 1.0f);
            float volume = glm::mix(0.0f, 1.0f, t);
            ChangeVolume(volume);
        }else{
            _isRisingUp = false;
            _timer = 0.0f;
        }
    }

}

void Sound::PlaySound(std::shared_ptr<Node> soundSourceNode, float adjVolume) {
    if(_playerNode == nullptr){
        _playerNode = NODESMANAGER.getNodeByName("player");
    }

    float volume = CalculateVolumeToPlayerDistance(soundSourceNode) * adjVolume;
    ChangeVolume(volume);

    if (ma_sound_start(&_sound) != MA_SUCCESS) {
        std::cerr << "Failed to start sound: " << _name << std::endl;
    } else {
        //std::cout << "Playing sound: " << _name << std::endl;
    }
}

void Sound::StopSound() {
    if (ma_sound_is_playing(&_sound)) {
        ma_sound_stop(&_sound);
        ma_sound_seek_to_pcm_frame(&_sound, 0);
    }
}

void Sound::PlaySoundSim(std::shared_ptr<Node> soundSourceNode) {

    if(_playerNode == nullptr){
        _playerNode = NODESMANAGER.getNodeByName("player");
    }

    float volume = CalculateVolumeToPlayerDistance(soundSourceNode) * _volume;

    ma_sound* sound = new ma_sound; // Dynamically allocate memory for the sound instance

    //std::cout << sound->pDataSource << std::endl;

    auto result = ma_sound_init_from_file(&AUDIOMANAGER._engine, _path.c_str(), 0, nullptr, nullptr, sound);
    if (result != MA_SUCCESS) {
        std::cerr << "Can't load sound file: " << _name << " at path: " << _path << std::endl;
        delete sound; // Clean up the allocated memory
        return;
    }

    // Set the volume based on the sound type
    ma_sound_set_volume(sound, volume * _constStartingVolumeMultiplier * (*_typeMultiplier));

    result = ma_sound_start(sound);

    if (result != MA_SUCCESS) {
        std::cerr << "Failed to start sound. Error: " << result << std::endl;
        ma_sound_uninit(sound);
        delete sound;
        return;
    }

    AUDIOMANAGER._activeSounds.push_back(sound);

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
    _volume = volume;
    ma_sound_set_volume(&_sound, volume * _constStartingVolumeMultiplier * (*_typeMultiplier));
}

void Sound::SetLooping(bool looping) {
    ma_sound_set_looping(&_sound, looping);
}

void Sound::RefreshVolume(){
    ma_sound_set_volume(&_sound, _volume * _constStartingVolumeMultiplier * (*_typeMultiplier));
}

void Sound::StopFadingAway(){
    _isFadingAway = false;
    _timer = 0.0f;
    StopSound();
}

float Sound::CalculateVolumeToPlayerDistance(std::shared_ptr<Node> soundSourceNode) {
    float distance = glm::distance(_playerNode->GetTransform()->GetPosition(), soundSourceNode->GetTransform()->GetPosition());

    if (distance <= 0) {
        return _constStartingVolumeMultiplier;
    } else if (distance < _maxSoundDistance) {
        float normalizedDistance = distance / _maxSoundDistance;
        float exponentialFade = exp(-normalizedDistance * 4);
        return _constStartingVolumeMultiplier * exponentialFade;
    } else {
        return 0;
    }
}


void Sound::FadeAway(float time) {
    _timer = 0;
    _timeToFadeAway = time;
    _isFadingAway = true;
}

void Sound::RiseUp(float time, shared_ptr<Node> sourceNode) {
    _volume = 0.0f;
    _timer = 0;
    _timeToRiseUp = time;
    PlaySound(sourceNode, 0);
    _isRisingUp = true;
}

void Sound::SetVolume(float volume) {
    _volume = volume;
}

float Sound::GetVolume() {
    return _volume;
}

void Sound::Reset() {
    StopSound();
    ma_sound_seek_to_pcm_frame(&_sound, 0);
}

void Sound::ResetTimer() {
    _timer = 0.0f;
}

void Sound::StopRisingUp() {
    _isRisingUp = false;
    _timer = 0.0f;
}
