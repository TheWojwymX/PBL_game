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

void PlayerAudioController::Initiate() {
    Component::Initiate();
}

void PlayerAudioController::Update() {

    if(_isWalking){
        _stepTimer += TIME.GetDeltaTime();
        if(_stepTimer >= _stepDelay){
            ResetStepTime();
        }
        Component::Update();
    }

    if(_ownerNode->GetTransform()->GetPosition().y < GAMEMANAGER._groundLevel && !_isPlayingCave){
        PlayCaveMusic();
    } else if(_ownerNode->GetTransform()->GetPosition().y >= GAMEMANAGER._groundLevel && _isPlayingCave){
        StopCaveMusic();
    } else if(_ownerNode->GetTransform()->GetPosition().y < GAMEMANAGER._groundLevel && _isPlayingCave && _isStoppingCaveMusic){
        ResetCaveMusic();
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
    RESOURCEMANAGER.GetSoundByName("JumpSound")->PlaySound(_ownerNode);
}

void PlayerAudioController::PlayRandomStepSound() {
    if (_stepSoundIDs.empty()) {
        throw std::runtime_error("The sound ID list is empty");
    }
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> dis(0, _stepSoundIDs.size() - 1);

    RESOURCEMANAGER.GetSoundByID(_stepSoundIDs[dis(gen)])->PlaySound(_ownerNode);
}

void PlayerAudioController::PlayCaveMusic() {
    RESOURCEMANAGER.GetSoundByName("CaveMusic")->RiseUp(5, _ownerNode);
    _isPlayingCave = true;
}

void PlayerAudioController::StopCaveMusic() {
    int fadeTime = 100; //I guess that sound is very quiet and that's why we need give horribly long time
    if(_caveTimer == 0.0f){
        _isStoppingCaveMusic = true;
        RESOURCEMANAGER.GetSoundByName("CaveMusic")->FadeAway(fadeTime);
    }
    if(_caveTimer < fadeTime){
        _caveTimer += TIME.GetDeltaTime();
    }else{
        ResetCaveMusic();
    }
}

void PlayerAudioController::ResetCaveMusic(){
    RESOURCEMANAGER.GetSoundByName("CaveMusic")->StopFadingAway();
    ma_sound_seek_to_pcm_frame(&RESOURCEMANAGER.GetSoundByName("CaveMusic")->_sound, 0);
    _caveTimer = 0.0f;
    _isPlayingCave = false;
    _isStoppingCaveMusic = false;
}
