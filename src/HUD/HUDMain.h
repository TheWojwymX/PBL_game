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

    ImageRenderer _crosshairImage;

    ImageRenderer _materialsBackground;
    ImageRenderer _metalImage;
    ImageRenderer _plasticImage;

    ImageRenderer _jetpackEmpty;
    ImageRenderer _jetpackBar;

    ImageRenderer _waveTimerGreen;
    ImageRenderer _waveTimerRed;
    ImageRenderer _waveArrowGreen;
    ImageRenderer _waveArrowRed;

    ImageRenderer _depthMeterBackground;

    std::vector<std::shared_ptr<ImageRenderer>> _baseHPImages;
    std::vector<std::shared_ptr<ImageRenderer>> _baseInsideImages;

    glm::vec2 hpTopRight = glm::vec2(0.952f, 0.952f);
    glm::vec2 hpBottomRight = glm::vec2(0.952f, 0.5f);
    glm::vec2 hpBottomLeft = glm::vec2(0.62f, 0.5f);
    glm::vec2 hpTopLeft = glm::vec2(0.62f, 0.952f);

    shared_ptr<Node> _playerNode;

    glm::vec3 interpolateColor(float percentFuel);

    bool _shouldShowCrosshair = false;
    bool _shouldShowHP = false;
    bool _shouldShowFuel = false;
    bool _shouldShowMaterials = false;
    bool _shouldShowDepth = false;
    bool _shouldShowPhaseInfo = false;

    ImageRenderer testowy;
    bool czyWyswietlacTestowy = false;
};


#endif //OPENGLGP_HUD_H
