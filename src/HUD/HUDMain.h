//
// Created by Jacek on 03.04.2024.
//

#define HUD HUDMain::getInstance()

#ifndef OPENGLGP_HUD_H
#define OPENGLGP_HUD_H

#include "Text/TextRenderer.h"
#include "Image/ImageRenderer.h"

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
};


#endif //OPENGLGP_HUD_H
