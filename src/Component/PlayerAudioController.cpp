//
// Created by Jacek on 14.04.2024.
//

#include "PlayerAudioController.h"
#include "Managers/NodesManager.h"

PlayerAudioController::PlayerAudioController() {
    _type = ComponentType::AUDIOSOURCE;
}

nlohmann::json PlayerAudioController::Serialize() {
    nlohmann::json data = Component::Serialize();
    return data;
}

void PlayerAudioController::Deserialize(const nlohmann::json &jsonData) {
    Component::Deserialize(jsonData);
}

void PlayerAudioController::initiate() {
    Component::initiate();
}

void PlayerAudioController::Update() {

    if(_isWalking){
        _stepTimer += TIME.GetDeltaTime();
        if(_stepTimer >= _stepDelay){
            ResetStepTime();
        }
        Component::Update();
    }
}

void PlayerAudioController::StartSteps(){
    ResetStepTime();
    _isWalking = true;
}

void PlayerAudioController::StopSteps(){
    _isWalking = false;
}

void PlayerAudioController::ResetStepTime(){
    PlayRandomStepSound();
    _stepTimer = 0;
}

void PlayerAudioController::PlayJumpSound(){
    RESOURCEMANAGER.GetSoundByName("JumpSound")->PlaySound();
}

void PlayerAudioController::PlayRandomStepSound() {
    if (_stepSoundIDs.empty()) {
        throw std::runtime_error("The sound ID list is empty");
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, _stepSoundIDs.size() - 1);

    RESOURCEMANAGER.GetSoundByID(_stepSoundIDs[dis(gen)])->PlaySound();
}
