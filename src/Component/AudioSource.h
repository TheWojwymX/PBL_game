//
// Created by Jacek on 14.04.2024.
//

#ifndef OPENGLGP_AUDIOSOURCE_H
#define OPENGLGP_AUDIOSOURCE_H

#include "Core/Component.h"

class AudioSource : public Component {
public:

    AudioSource();

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

    void initiate() override;

    void Update() override;
};


#endif //OPENGLGP_AUDIOSOURCE_H
