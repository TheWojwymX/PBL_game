//
// Created by Jacek on 13.05.2024.
//

#include "Turret.h"

Turret::Turret() {
    _type = ComponentType::TURRET;
}

nlohmann::json Turret::Serialize() {
    nlohmann::json data;
    return data;
}

void Turret::Deserialize(const nlohmann::json &jsonData) {
    Component::Deserialize(jsonData);
}