//
// Created by Jacek on 14.04.2024.
//

#include "AudioSource.h"
#include "Managers/NodesManager.h"

AudioSource::AudioSource() {
    _type = ComponentType::AUDIOSOURCE;
}

nlohmann::json AudioSource::Serialize() {
    nlohmann::json data = Component::Serialize();
    return data;
}

void AudioSource::Deserialize(const nlohmann::json &jsonData) {
    Component::Deserialize(jsonData);
}

void AudioSource::initiate() {
    Component::initiate();
}

void AudioSource::Update() {

    if(_isWalking){
        _stepTimer += TIME.GetDeltaTime();
        if(_stepTimer >= _stepDelay){
            ResetStepTime();
        }
        Component::Update();
    }
}

void AudioSource::StartSteps(){
    ResetStepTime();
    _isWalking = true;
}

void AudioSource::StopSteps(){
    _isWalking = false;
}

void AudioSource::ResetStepTime(){
    PlayRandomStepSound();
    _stepTimer = 0;
}

void AudioSource::PlayRandomStepSound() {
    if (_stepSoundIDs.empty()) {
        throw std::runtime_error("The sound ID list is empty");
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, _stepSoundIDs.size() - 1);

    RESOURCEMANAGER.GetSoundByID(_stepSoundIDs[dis(gen)])->PlaySound();
}
