//
// Created by Jacek on 14.04.2024.
//

#ifndef OPENGLGP_PLAYERAUDIOCONTROLLER_H
#define OPENGLGP_PLAYERAUDIOCONTROLLER_H

#include "Core/Component.h"
#include "Managers/ResourceManager.h"
#include <random>
#include "Core/Time.h"

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

    std::vector<int> _stepSoundIDs = {1,2,3, 5, 6, 7, 8, 9, 10, 11};

    bool _isWalking = false;

    float _stepDelay = 0.5;

    void StartSteps();

    void StopSteps();

    void PlayJumpSound();
};


#endif //OPENGLGP_PLAYERAUDIOCONTROLLER_H
