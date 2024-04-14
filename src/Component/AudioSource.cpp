//
// Created by Jacek on 14.04.2024.
//

#include "AudioSource.h"

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
