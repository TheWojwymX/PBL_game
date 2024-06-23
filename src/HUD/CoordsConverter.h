//
// Created by Jacek on 14.06.2024.
//

#ifndef SANDBOX_COORDSCONVERTER_H
#define SANDBOX_COORDSCONVERTER_H
#include "glm/vec2.hpp"

class CoordsConverter {
public:
    static glm::vec2 ConvertCoords(const glm::vec2 &coords);

    static glm::vec2 ConvertTextCoords(const glm::vec2 &coords);
};


#endif //SANDBOX_COORDSCONVERTER_H
