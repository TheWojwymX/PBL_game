//
// Created by Jacek on 14.04.2024.
//

#ifndef OPENGLGP_PLAYERAUDIOCONTROLLER_H
#define OPENGLGP_PLAYERAUDIOCONTROLLER_H

#include "Core/Component.h"
#include "Managers/ResourceManager.h"
#include <random>
#include "Core/Time.h"
#include "BatchRandomGenerator.h"

class PlayerAudioController : public Component {
public:

    PlayerAudioController();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void Initiate() override;

    void Update() override;

    void PlayRandomStepSound();

    void ResetStepTime();

    float _stepTimer = 0;
    BatchRandomGenerator _soundStepRand;

    std::vector<int> _stepSoundIDs = {1, 2, 3, 4, 5};

    bool _isWalking = false;

    float _stepDelay = 0.5;

    void StartSteps();

    void StopSteps();

    void PlayJumpSound();

    float _caveTimer = 0.0f;
    bool _isPlayingCave;
    bool _isStoppingCaveMusic = false;

    void PlayCaveMusic();
    void StopCaveMusic();

    void ResetCaveMusic();

    void ChangeMusicToDigPhase();
    void ChangeMusicToBattlePhase();

    void PlayWindAmbient();

    void StopWindAmbient();

    bool _isPlayingWindAmbient = false;
    bool _isPlayingBattleMusic = false;

    bool _isInDigPhase = false;
};


#endif //OPENGLGP_PLAYERAUDIOCONTROLLER_H
