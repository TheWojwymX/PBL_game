//
// Created by Jacek on 03.04.2024.
//

#define HUD HUDMain::getInstance()

#ifndef OPENGLGP_HUD_H
#define OPENGLGP_HUD_H

#include "Text/TextRenderer.h"
#include "Image/ImageRenderer.h"
#include "core/Time.h"
#include "Managers/DomeManager.h"
#include "Managers/GameManager.h"
#include <GLFW/glfw3.h>

class HUDMain {

public:

    ~HUDMain() = default;

    HUDMain() = default;

    static HUDMain &getInstance();

    void Init();

    void Update();

    HUDMain(const HUDMain &) = delete;

    HUDMain &operator=(const HUDMain &) = delete;

    ImageRenderer _hpEmptyImage;
    ImageRenderer _hpFullImage;
    ImageRenderer _crosshairImage;

    std::vector<std::shared_ptr<ImageRenderer>> _baseHPImages;

    glm::vec2 hpTopRight = glm::vec2(0.952f, 0.952f);
    glm::vec2 hpBottomRight = glm::vec2(0.952f, 0.5f);
    glm::vec2 hpBottomLeft = glm::vec2(0.62f, 0.5f);
    glm::vec2 hpTopLeft = glm::vec2(0.62f, 0.952f);

    float _hpTextureX = 0;
    float _hpTextureY = 0;
    float _hpSpriteSheetWidth = 256;
    float _hpSpriteSheetHeight = 64;
    float _hpSpriteWidth = 64;
    float _hpSpriteHeight = 64;
    float _hpSpriteTimer = 0.0;
    float _hpSpriteInterval = 0.3f;
    int _hpSpriteCurrentFrame = 0;
};


#endif //OPENGLGP_HUD_H
