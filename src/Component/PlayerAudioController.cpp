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

    if(GAMEMANAGER._isInMainMenu) return;

    if(_isWalking){
        _stepTimer += TIME.GetDeltaTime();
        if(_stepTimer >= _stepDelay){
            ResetStepTime();
        }
        Component::Update();
    }

    if(_ownerNode->GetTransform()->GetPosition().y < GAMEMANAGER._groundLevel && !_isPlayingCave && _isInDigPhase){
        PlayCaveMusic();
        StopWindAmbient();
    } else if(_ownerNode->GetTransform()->GetPosition().y >= GAMEMANAGER._groundLevel && _isPlayingCave && _isInDigPhase){
        StopCaveMusic();
        if(!_isPlayingWindAmbient){
            PlayWindAmbient();
        }
    } else if(_ownerNode->GetTransform()->GetPosition().y < GAMEMANAGER._groundLevel && _isPlayingCave && _isStoppingCaveMusic && _isInDigPhase){
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
    if(GAMEMANAGER._currentPhase == DEFEND) return;
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

void PlayerAudioController::ChangeMusicToDigPhase() {
    _isInDigPhase = true;

    if(!_isPlayingWindAmbient){
        PlayWindAmbient();
    }

    if(_isPlayingBattleMusic){
        RESOURCEMANAGER.GetSoundByName("BattleMusic")->SetLooping(false);
        RESOURCEMANAGER.GetSoundByName("BattleMusic")->FadeAway(2);
        _isPlayingBattleMusic = false;
    }
}

void PlayerAudioController::ChangeMusicToBattlePhase() {
    _isInDigPhase = false;

    StopWindAmbient();
    StopCaveMusic();
    RESOURCEMANAGER.GetSoundByName("BattleMusic")->SetLooping(true);
    RESOURCEMANAGER.GetSoundByName("BattleMusic")->RiseUp(5, _ownerNode);
    _isPlayingBattleMusic = true;
}

void PlayerAudioController::PlayWindAmbient(){
    RESOURCEMANAGER.GetSoundByName("DesertAmbient")->SetLooping(true);
    RESOURCEMANAGER.GetSoundByName("DesertAmbient")->StopFadingAway();
    RESOURCEMANAGER.GetSoundByName("DesertAmbient")->RiseUp(5, _ownerNode);
    _isPlayingWindAmbient = true;
}

void PlayerAudioController::StopWindAmbient(){
    RESOURCEMANAGER.GetSoundByName("DesertAmbient")->SetLooping(false);
    RESOURCEMANAGER.GetSoundByName("DesertAmbient")->StopRisingUp();
    RESOURCEMANAGER.GetSoundByName("DesertAmbient")->FadeAway(5);
    _isPlayingWindAmbient = false;
}
