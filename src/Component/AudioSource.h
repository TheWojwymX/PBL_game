//
// Created by Jacek on 14.04.2024.
//

#ifndef OPENGLGP_AUDIOSOURCE_H
#define OPENGLGP_AUDIOSOURCE_H

#include "Core/Component.h"
#include "Managers/ResourceManager.h"
#include <random>
#include "Core/Time.h"

class AudioSource : public Component {
public:

    AudioSource();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void initiate() override;

    void Update() override;

    void PlayRandomStepSound();

    void ResetStepTime();

    float _stepTimer = 0;

    std::vector<int> _stepSoundIDs = {1,2,3};

    bool _isWalking = false;

    float _stepDelay = 0.5;

    void StartSteps();

    void StopSteps();
};


#endif //OPENGLGP_AUDIOSOURCE_H
