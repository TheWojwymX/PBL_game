//
// Created by Jacek on 14.04.2024.
//

#include "PlayerAudioController.h"
#include "Managers/NodesManager.h"

PlayerAudioController::PlayerAudioController():_soundStepRand(30,0,4){
    _type = ComponentType::AUDIOSOURCE;
    InitSounds();
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
    _jumpSound->PlaySound(_ownerNode);
}

void PlayerAudioController::PlayRandomStepSound() {
    if (_stepSoundIDs.empty()) {
        throw std::runtime_error("The sound ID list is empty");
    }

    _stepSounds[_soundStepRand.GetRandomInt()]->PlaySound(_ownerNode);
}

void PlayerAudioController::PlayCaveMusic() {
    if(GAMEMANAGER._currentPhase == DEFEND) return;
    _caveMusic->RiseUp(5, _ownerNode);
    _isPlayingCave = true;
}

void PlayerAudioController::StopCaveMusic() {
    int fadeTime = 100; //I guess that sound is very quiet and that's why we need give horribly long time
    if(_caveTimer == 0.0f){
        _isStoppingCaveMusic = true;
        _caveMusic->FadeAway(fadeTime);
    }
    if(_caveTimer < fadeTime){
        _caveTimer += TIME.GetDeltaTime();
    }else{
        ResetCaveMusic();
    }
}

void PlayerAudioController::ResetCaveMusic(){
    _caveMusic->StopFadingAway();
    ma_sound_seek_to_pcm_frame(&_caveMusic->_sound, 0);
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
        _battleMusic->SetLooping(false);
        _battleMusic->FadeAway(2);
        _isPlayingBattleMusic = false;
    }
}

void PlayerAudioController::ChangeMusicToBattlePhase() {
    _isInDigPhase = false;

    StopWindAmbient();
    StopCaveMusic();
    _battleMusic->SetLooping(true);
    _battleMusic->RiseUp(5, _ownerNode);
    _isPlayingBattleMusic = true;
}

void PlayerAudioController::PlayWindAmbient(){
    _desertAmbient->SetLooping(true);
    _desertAmbient->StopFadingAway();
    _desertAmbient->RiseUp(5, _ownerNode);
    _isPlayingWindAmbient = true;
}

void PlayerAudioController::StopWindAmbient(){
    _desertAmbient->SetLooping(false);
    _desertAmbient->StopRisingUp();
    _desertAmbient->FadeAway(5);
    _isPlayingWindAmbient = false;
}

void PlayerAudioController::InitSounds()
{
    _jumpSound = RESOURCEMANAGER.GetSoundByName("JumpSound");
    for (int id : _stepSoundIDs) {
        _stepSounds.push_back(RESOURCEMANAGER.GetSoundByID(id));
    }
    _caveMusic = RESOURCEMANAGER.GetSoundByName("CaveMusic");
    _battleMusic = RESOURCEMANAGER.GetSoundByName("BattleMusic");
    _desertAmbient = RESOURCEMANAGER.GetSoundByName("DesertAmbient");
}
