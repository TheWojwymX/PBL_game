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
    Component::Update();
}

void AudioSource::PlayRandomStepSound() {
    if (_stepSoundIDs.empty()) {
        throw std::runtime_error("The sound ID list is empty");
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, _stepSoundIDs.size() - 1);

    //RESOURCEMANAGER.GetSoundByID(_stepSoundIDs[dis(gen)])->PlaySound();

    std::cout << this->_ownerNode->GetComponent<AudioSource>()->_id << std::endl;
}
