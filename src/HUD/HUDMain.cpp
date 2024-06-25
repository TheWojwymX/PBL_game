#pragma once

#include "HUDMain.h"

HUDMain& HUDMain::getInstance() {
    static HUDMain instance;
    return instance;
}

void HUDMain::Init() {
    //text
    TEXTRENDERER.Init();

    _crosshairImage.Init("res/Images/crosshair041.png", glm::vec2(0, 0), 0, true, false);
    _materialsBackground.Init("res/Images/HUD/background_3.1.png", CoordsConverter::ConvertCoords(glm::vec2(28, 1051)), CoordsConverter::ConvertCoords(glm::vec2(177, 967)), true, false);
    _plasticImage.Init("res/Images/HUD/plastic_icon.png", CoordsConverter::ConvertCoords(glm::vec2(74, 1009)), 0, true, false);
    _depthMeterBackground.Init("res/Images/HUD/depth0.png", CoordsConverter::ConvertCoords(glm::vec2(1662, 1051)), CoordsConverter::ConvertCoords(glm::vec2(1891, 967)), true, false);
    _waveTimerGreen.Init("res/Images/WaveTimer/zegar_zielony.png", CoordsConverter::ConvertCoords(glm::vec2(124, 139)), 0, true, true);
    _waveTimerRed.Init("res/Images/WaveTimer/zegar_czerwony.png", CoordsConverter::ConvertCoords(glm::vec2(124, 139)), 0, true, true);
    _waveArrowGreen.Init("res/Images/WaveTimer/strzalka_zielona.png", CoordsConverter::ConvertCoords(glm::vec2(124, 139)), 90, true, true);
    _waveArrowRed.Init("res/Images/WaveTimer/strzalka_czerwona.png", CoordsConverter::ConvertCoords(glm::vec2(124, 139)), 90, true, true);
    testowy.Init("res/Images/HUD/testowy.png", glm::vec2(-50, -50), glm::vec2(50, 50), true, false);


    _tutorialBackground.Init("res/Images/HUD/tutorial_window.png", CoordsConverter::ConvertCoords(glm::vec2(206, 1051)), CoordsConverter::ConvertCoords(glm::vec2(1633, 967)), true, false);

    for(int i = 0; i <= 20; i++){
        shared_ptr<ImageRenderer> jetpack = make_shared<ImageRenderer>();
        std::string path = "res/Images/HUD/Jetpack/fuel (" + std::to_string(i+1) + ").png";
        //jetpack->Init(path.c_str(), CoordsConverter::ConvertCoords(glm::vec2(1845, 570)), 90, true, true);
        jetpack->Init(path.c_str(), CoordsConverter::ConvertCoords(glm::vec2(1726, 946)), CoordsConverter::ConvertCoords(glm::vec2(1894, 319)), true, false);
        _jetpackImages.push_back(jetpack);
    }


    for(int i = 0; i <= 20; i++){
        shared_ptr<ImageRenderer> hp = make_shared<ImageRenderer>();
        std::string path = "res/Images/HUD/BaseHP/Border/bar_" + std::to_string(5*i) + ".png";
        hp->Init(path.c_str(), CoordsConverter::ConvertCoords(glm::vec2(1790, 138)), 0, true, true);
        _baseHPImages.push_back(hp);
    }

    for(int i = 0; i <= 3; i++){
        shared_ptr<ImageRenderer> hp = make_shared<ImageRenderer>();
        std::string path = "res/Images/HUD/BaseHP/InsideImages/inside" + std::to_string(i) + ".png";
        hp->Init(path.c_str(), CoordsConverter::ConvertCoords(glm::vec2(1790, 138)), 0, true, true);
        _baseInsideImages.push_back(hp);
    }

    _playerNode = NODESMANAGER.getNodeByName("player");
}

void HUDMain::Update() {

    //images
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    if(INPUT.IsKeyPressed(GLFW_KEY_KP_9)){
        czyWyswietlacTestowy = !czyWyswietlacTestowy;
    }
    if(czyWyswietlacTestowy){
        testowy.Render();
    }

    glm::vec leftDownCorner = glm::vec3(0.7614583, 0.5564815, 0.0);
    glm::vec rightTopCorner = glm::vec3(0.9723958, 0.9314815, 0.0);

    leftDownCorner = glm::vec3(0.78, 0.58, 0.0);
    rightTopCorner = glm::vec3(0.959, 0.899, 0.0);

    float actualDomeHP = DOMEMANAGER.GetDomeHP();
    float maxHP = DOMEMANAGER.GetDomeMaxHP();
    float percentHP = actualDomeHP / maxHP * 100;

    //std::cout << actualDomeHP << "   " << maxHP << "   " << percentHP << "   " << (actualDomeHP/maxHP) * 100 << std::endl;
    if(_shouldShowHP && _isAfterTutorialHP){

        for(shared_ptr<ImageRenderer> hpBorderImage : _baseHPImages){
            hpBorderImage->SetScale(glm::vec2(_defaultHPScale, _defaultHPScale));
        }
        for(shared_ptr<ImageRenderer> hpInsideImage : _baseInsideImages){
            hpInsideImage->SetScale(glm::vec2(_defaultInnerHPScale, _defaultInnerHPScale));
        }

        PlayHPBump();

        if(percentHP <= 0){
            _baseInsideImages[3]->Render();
        }
        else if(percentHP < 33){
            _baseInsideImages[2]->Render();
        }
        else if(percentHP < 66){
            _baseInsideImages[1]->Render();
        }
        else{
            _baseInsideImages[0]->Render();
        }

        int index = (percentHP - 1) / 5;

        if(index != _previousIndex){
            _previousIndex = index;
            _isHPInAnim = true;
        }

        if (percentHP <= 0) {
            _baseHPImages[0]->Render();
        } else if (percentHP == 100) {
            _baseHPImages[20]->Render();
        } else {
            _baseHPImages[index + 1]->Render();
        }

        for(int i = 0; i < _attackSymbols.size(); i++)
        {
            auto camera = ComponentsManager::getInstance().GetComponentByID<Camera>(2);
            auto enemyPosition = _attackSymbols[i].second->GetOwnerPosition();

            glm::vec3 directionToEnemy = glm::normalize(glm::vec3(enemyPosition.x - camera->GetPosition().x, 0.0f, enemyPosition.z - camera->GetPosition().z));
            glm::vec3 forward = glm::normalize(glm::vec3(camera->GetFrontVector().x, 0.0f, camera->GetFrontVector().z));
            float angle = glm::degrees(glm::acos(glm::dot(forward, directionToEnemy)));

            if (glm::cross(forward, directionToEnemy).y < 0) {
                angle = 360.0f - angle;
            }

            _attackSymbols[i].first->SetRotationAngle(angle + 90);
            _attackSymbols[i].first->Render();
        }

    }
    if(_shouldShowCrosshair && _isAfterTutorialCrosshair){
        _crosshairImage.Render();
    }

    if(_isTutorialNeededAtMoment && _shouldShowTutorial){
        _tutorialBackground.Render();
        TEXTRENDERER.RenderTextCentered(_actualText, 0.0f, -0.865, 0.4f, glm::vec3(0.0f, 0.0f, 0.0f));
    }

    //materials
    if(_shouldShowMaterials && _isAfterTutorialMaterials){
        _materialsBackground.Render();

        _plasticImage.SetScale(glm::vec2(_defaultPlasticScale, _defaultPlasticScale));
        PlayPlasticBump();
        _plasticImage.Render();

        TEXTRENDERER.RenderText(to_string(GAMEMANAGER.GetPlastic()), -0.8833333, -0.879, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    //jetpack
    if(_shouldShowFuel && _isAfterTutorialFuel){
        int imageIndex = static_cast<int>((_playerNode->GetComponent<PlayerController>()->_jetpackFuel / _playerNode->GetComponent<PlayerController>()->_maxJetpackFuel) * 20.0f);
        _jetpackImages[imageIndex]->Render();
    }


    //depthMeter
    if(_shouldShowDepth && _isAfterTutorialDepth){
        _depthMeterBackground.Render();
        TEXTRENDERER.RenderText(to_string(-(int)std::floor(GAMEMANAGER._groundLevel - _playerNode->GetTransform()->GetPosition().y)), 0.82, -0.8833333, 0.5f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    //timer
    if(_shouldShowPhaseInfo && _isAfterTutorialPhaseInfo){

        _waveTimerGreen.SetScale(glm::vec2(_defaultWaveTimerScale, _defaultWaveTimerScale));
        _waveTimerRed.SetScale(glm::vec2(_defaultWaveTimerScale, _defaultWaveTimerScale));
        PlayWaveTimerBump();
        WaveTimerGUIManager();

        //TEXTRENDERER.RenderText("TTN: " + to_string(GAMEMANAGER.phaseTime - GAMEMANAGER.currentTime), -0.97f, 0.88f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void HUDMain::WaveTimerGUIManager() {
    auto currentPhase = GAMEMANAGER.GetPhase();
    float proportion = GAMEMANAGER.GetPhaseProgress();

    float degrees = (proportion * 360.0f);

    float roundedDegrees = -((int(degrees/22.5f) * 22.5f) - 90.0f);
    
    if (_oldClockDegrees <= -180.0f && _oldClockDegrees != roundedDegrees){
        _isWaveTimerInAnim = true;
        RESOURCEMANAGER.GetSoundByName("Tick")->PlaySound(_playerNode);
    }
    _oldClockDegrees = roundedDegrees;

    if (currentPhase == Phase::DIG) {
        _waveTimerGreen.Render();
        _waveArrowRed.Render();

        _waveArrowRed.SetRotationAngle(roundedDegrees);

        if (_clockTimer < _clockInterval) {
            _clockTimer += TIME.GetDeltaTime();
        }
        else {
            _clockTimer = 0;
        }
    }
    else if (currentPhase == Phase::DEFEND) {
        _waveTimerRed.Render();
        _waveArrowGreen.Render();

        _waveArrowGreen.SetRotationAngle(roundedDegrees);

        if (_clockTimer < _clockInterval) {
            _clockTimer += TIME.GetDeltaTime();
        }
        else {
            _clockTimer = 0;
        }
    }
}

void HUDMain::DisableHUD() {
    _shouldShowCrosshair = false;
    _shouldShowHP = false;
    _shouldShowFuel = false;
    _shouldShowMaterials = false;
    _shouldShowDepth = false;
    _shouldShowPhaseInfo = false;
    _shouldShowTutorial = false;
}

void HUDMain::EnableHUD() {
    if (_isAfterTutorialCrosshair) {
        _shouldShowCrosshair = true;
    }

    if (_isAfterTutorialHP) {
        _shouldShowHP = true;
    }

    if (_isAfterTutorialFuel) {
        _shouldShowFuel = true;
    }

    if (_isAfterTutorialMaterials) {
        _shouldShowMaterials = true;
    }

    if (_isAfterTutorialDepth) {
        _shouldShowDepth = true;
    }

    if (_isAfterTutorialPhaseInfo) {
        _shouldShowPhaseInfo = true;
    }

    _shouldShowTutorial = true;
}

void HUDMain::PlayPlasticBump() {
    if (_isPlasticInAnim) {
        if (_plasticAnimTimer < _timeOfPlasticAnim) {
            _plasticAnimTimer += TIME.GetDeltaTime();

            if (_plasticAnimTimer < _timeOfPlasticAnim / 2.0f) {
                float t = _plasticAnimTimer / (_timeOfPlasticAnim / 2.0f);
                t = glm::clamp(t, 0.0f, 1.0f);
                float scale = glm::mix(_defaultPlasticScale, _maxPlasticScale, t);
                _plasticImage.SetScale(glm::vec2(scale, scale));
            } else {
                float t = (_plasticAnimTimer - _timeOfPlasticAnim / 2.0f) / (_timeOfPlasticAnim / 2.0f);
                t = glm::clamp(t, 0.0f, 1.0f);
                float scale = glm::mix(_maxPlasticScale, _defaultPlasticScale, t);
                _plasticImage.SetScale(glm::vec2(scale, scale));
            }
        } else {
            _plasticAnimTimer = 0;
            _isPlasticInAnim = false;
        }
    }
}

void HUDMain::Reset() {

}

void HUDMain::PlayWaveTimerBump() {
    if (_isWaveTimerInAnim) {
        if (_waveTimerAnimTimer < _timeOfWaveTimerAnim) {
            _waveTimerAnimTimer += TIME.GetDeltaTime();

            if (_waveTimerAnimTimer < _timeOfWaveTimerAnim / 2.0f) {
                float t = _waveTimerAnimTimer / (_timeOfWaveTimerAnim / 2.0f);
                t = glm::clamp(t, 0.0f, 1.0f);
                float backgroundScale = glm::mix(_defaultWaveTimerScale, _maxWaveTimerScale, t);
                float arrowScale = glm::mix(_defaultTimerArrowScale, _maxTimerArrowScale, t);
                _waveTimerGreen.SetScale(glm::vec2(backgroundScale, backgroundScale));
                _waveTimerGreen.SetScale(glm::vec2(backgroundScale, backgroundScale));
                _waveArrowRed.SetScale(glm::vec2(arrowScale, arrowScale));
                _waveArrowGreen.SetScale(glm::vec2(arrowScale, arrowScale));
            } else {
                float t = (_waveTimerAnimTimer - _timeOfWaveTimerAnim / 2.0f) / (_timeOfWaveTimerAnim / 2.0f);
                t = glm::clamp(t, 0.0f, 1.0f);
                float backgroundScale = glm::mix(_maxWaveTimerScale, _defaultWaveTimerScale, t);
                float arrowScale = glm::mix(_maxTimerArrowScale, _defaultTimerArrowScale, t);
                _waveTimerGreen.SetScale(glm::vec2(backgroundScale, backgroundScale));
                _waveTimerGreen.SetScale(glm::vec2(backgroundScale, backgroundScale));
                _waveArrowRed.SetScale(glm::vec2(arrowScale, arrowScale));
                _waveArrowGreen.SetScale(glm::vec2(arrowScale, arrowScale));
            }
        } else {
            _waveTimerAnimTimer = 0;
            _isWaveTimerInAnim = false;
        }
    }
}

void HUDMain::PlayHPBump() {
    if (_isHPInAnim) {
        if (_HPAnimTimer < _timeOfHPAnim) {
            _HPAnimTimer += TIME.GetDeltaTime();

            if (_HPAnimTimer < _timeOfHPAnim / 2.0f) {
                float t = _HPAnimTimer / (_timeOfHPAnim / 2.0f);
                t = glm::clamp(t, 0.0f, 1.0f);
                float backgroundScale = glm::mix(_defaultHPScale, _maxHPScale, t);
                float innerScale = glm::mix(_defaultInnerHPScale, _maxInnerHPScale, t);
                for(shared_ptr<ImageRenderer> hpBorderImage : _baseHPImages){
                    hpBorderImage->SetScale(glm::vec2(backgroundScale, backgroundScale));
                }
                for(shared_ptr<ImageRenderer> hpInsideImage : _baseInsideImages){
                    hpInsideImage->SetScale(glm::vec2(innerScale, innerScale));
                }
            } else {
                float t = (_HPAnimTimer - _timeOfHPAnim / 2.0f) / (_timeOfHPAnim / 2.0f);
                t = glm::clamp(t, 0.0f, 1.0f);
                float backgroundScale = glm::mix(_maxHPScale, _defaultHPScale, t);
                float innerScale = glm::mix(_maxInnerHPScale, _defaultInnerHPScale, t);
                for(shared_ptr<ImageRenderer> hpBorderImage : _baseHPImages){
                    hpBorderImage->SetScale(glm::vec2(backgroundScale, backgroundScale));
                }
                for(shared_ptr<ImageRenderer> hpInsideImage : _baseInsideImages){
                    hpInsideImage->SetScale(glm::vec2(innerScale, innerScale));
                }
            }
        } else {
            _HPAnimTimer = 0;
            _isHPInAnim = false;
        }
    }
}
