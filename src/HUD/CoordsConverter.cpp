//
// Created by Jacek on 14.06.2024.
//

#include "CoordsConverter.h"


glm::vec2 CoordsConverter::ConvertCoords(const glm::vec2& coords) {
    float x = (coords.x / 1920.0f) * 100.0f - 50.0f;
    float y = (1.0f - (coords.y / 1080.0f)) * 100.0f - 50.0f;
    return glm::vec2(x, y);
}

glm::vec2 CoordsConverter::ConvertTextCoords(const glm::vec2& coords) {
    float x = (coords.x / 1920.0f) * 2.0f - 1.0f;
    float y = (1.0f - (coords.y / 1080.0f)) * 2.0f - 1.0f;
    return glm::vec2(x, y);
}