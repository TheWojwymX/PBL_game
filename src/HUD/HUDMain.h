#pragma once
#define HUD HUDMain::getInstance()
#include "Text/TextRenderer.h"
#include "HUD/Image/ImageRenderer.h"
#include "core/Time.h"
#include "Managers/DomeManager.h"
#include "Managers/GameManager.h"
#include <GLFW/glfw3.h>
#include "CoordsConverter.h"

class Enemy;
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
    ImageRenderer _plasticImage;

    ImageRenderer _waveTimerGreen;
    ImageRenderer _waveTimerRed;
    ImageRenderer _waveArrowRed;
    ImageRenderer _waveArrowGreen;

    ImageRenderer _depthMeterBackground;

    std::vector<std::pair<std::shared_ptr<ImageRenderer>, std::shared_ptr<Enemy>>> _attackSymbols;

    std::vector<std::shared_ptr<ImageRenderer>> _jetpackImages;

    std::vector<std::shared_ptr<ImageRenderer>> _baseHPImages;
    std::vector<std::shared_ptr<ImageRenderer>> _baseInsideImages;

    glm::vec2 hpTopRight = glm::vec2(0.952f, 0.952f);
    glm::vec2 hpBottomRight = glm::vec2(0.952f, 0.5f);
    glm::vec2 hpBottomLeft = glm::vec2(0.62f, 0.5f);
    glm::vec2 hpTopLeft = glm::vec2(0.62f, 0.952f);

    bool _shouldShowCrosshair = false;
    bool _shouldShowHP = false;
    bool _shouldShowFuel = false;
    bool _shouldShowMaterials = false;
    bool _shouldShowDepth = false;
    bool _shouldShowPhaseInfo = false;

    bool _isAfterTutorialCrosshair = false;
    bool _isAfterTutorialHP = false;
    bool _isAfterTutorialFuel = false;
    bool _isAfterTutorialMaterials = false;
    bool _isAfterTutorialDepth = false;
    bool _isAfterTutorialPhaseInfo = false;


    ImageRenderer testowy;
    ImageRenderer _tutorialBackground;
    bool _isTutorialNeededAtMoment = false;
    bool _shouldShowTutorial = false;
    string _actualText;

    bool czyWyswietlacTestowy = false;

    void WaveTimerGUIManager();

    float _clockTimer = 0.0f;
    float _clockInterval = 0.5f;

    void DisableHUD();
    void EnableHUD();

    bool _isPlasticInAnim = false;
    float _plasticAnimTimer = 0.0f;
    float _timeOfAnim = 0.2;
    float _maxScale = 2.3f;
    float _defaultScale = 1.7f;

    void Reset();

private:
    std::shared_ptr<Node> _playerNode;

    void PlayPlasticBump();
};
