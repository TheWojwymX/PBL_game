//
// Created by Jacek on 03.04.2024.
//

#define HUD HUDMain::getInstance()

#ifndef OPENGLGP_HUD_H
#define OPENGLGP_HUD_H

#include "Text/TextRenderer.h"
#include "Image/ImageRenderer.h"
#include "core/Time.h"

//te,p
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
    ImageRenderer _animatedImage;

    glm::vec2 hpTopRight = glm::vec2(0.952f, 0.952f);
    glm::vec2 hpBottomRight = glm::vec2(0.952f, 0.5f);
    glm::vec2 hpBottomLeft = glm::vec2(0.62f, 0.5f);
    glm::vec2 hpTopLeft = glm::vec2(0.62f, 0.952f);
};


#endif //OPENGLGP_HUD_H
