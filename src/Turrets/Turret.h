//
// Created by Jacek on 13.05.2024.
//

#ifndef SANDBOX_TURRET_H
#define SANDBOX_TURRET_H


#include "Core/Component.h"

class Turret : public Component, public std::enable_shared_from_this<Turret> {

public:
    Turret();

private:

    nlohmann::json Serialize() override;

    void Deserialize(const nlohmann::json &jsonData) override;

};


#endif //SANDBOX_TURRET_H
