#pragma once
#define HUD HUDMain::getInstance()
#include "Text/TextRenderer.h"
#include "HUD/Image/ImageRenderer.h"
#include "core/Time.h"
#include "Managers/DomeManager.h"
#include "Managers/GameManager.h"
#include <GLFW/glfw3.h>
#include "CoordsConverter.h"
#include "BatchRandomGenerator.h"

class Enemy;

struct MaterialText {
    glm::vec2 pos;     
    glm::vec4 color;   
    std::string text; 

    MaterialText() : pos(0.0f), color(1.0f), text("") {}

    MaterialText(const glm::vec2& position, const glm::vec4& textColor, const std::string& textContent)
        : pos(position), color(textColor), text(textContent) {}
};

class HUDMain {

public:

    ~HUDMain() = default;

    HUDMain();

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
    float _oldClockDegrees = 0.0f;

    void DisableHUD();
    void EnableHUD();

    void PlayPlasticBump();
    bool _isPlasticInAnim = false;
    float _plasticAnimTimer = 0.0f;
    float _timeOfPlasticAnim = 0.2;
    float _maxPlasticScale = 2.3f;
    float _defaultPlasticScale = 1.7f;

    void PlayWaveTimerBump();
    bool _isWaveTimerInAnim = false;
    float _waveTimerAnimTimer = 0.0f;
    float _timeOfWaveTimerAnim = 0.2;
    float _maxWaveTimerScale = 2.3f;
    float _defaultWaveTimerScale = 2.0f;
    float _maxTimerArrowScale = 1.1f;
    float _defaultTimerArrowScale = 1.0f;

    void PlayHPBump();
    bool _isHPInAnim = false;
    float _HPAnimTimer = 0.0f;
    float _timeOfHPAnim = 0.2;
    float _maxHPScale = 2.3f;
    float _defaultHPScale = 2.0f;
    float _maxInnerHPScale = 2.3f;
    float _defaultInnerHPScale = 2.0f;
    float _maxHPArrowsScale = 1.3f;
    float _defaultHPArrowsScale = 1.0f;

    int _previousIndex = 20;

    void Reset();
    void AddMaterialText(int value);
private:
    std::shared_ptr<Node> _playerNode;
    std::vector<MaterialText> _materialTexts;
    float _materialTextVelocity = 1.0f;
    float _materialTextDisolve = 1.0f;
    BatchRandomGenerator _yoffsetRand;

    std::vector<int> _tickSoundsIDs = {26, 31, 32, 33, 34};
    int _actualTickID = 0;

    void UpdateMaterialTexts();
    void RenderMaterialTexts();


};
