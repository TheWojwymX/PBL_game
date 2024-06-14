#pragma once

#include "HUDMain.h"

HUDMain& HUDMain::getInstance() {
    static HUDMain instance;
    return instance;
}

glm::vec2 HUDMain::ConvertCoords(const glm::vec2& coords) {
    float x = (coords.x / 1920.0f) * 100.0f - 50.0f;
    float y = (1.0f - (coords.y / 1080.0f)) * 100.0f - 50.0f;
    return glm::vec2(x, y);
}

void HUDMain::Init() {

    //text
    TEXTRENDERER.Init();

    _crosshairImage.Init("res/Images/crosshair041.png", glm::vec2(0, 0), 0, true, false);
    _materialsBackground.Init("res/Images/HUD/materials_background.png", ConvertCoords(glm::vec2(28, 1051)), ConvertCoords(glm::vec2(314, 967)), true, false);
    _plasticImage.Init("res/Images/HUD/plastic_icon.png", ConvertCoords(glm::vec2(44, 1039)), ConvertCoords(glm::vec2(103, 979)), true, false);
    _metalImage.Init("res/Images/HUD/metal_icon.png", ConvertCoords(glm::vec2(179, 1039)), ConvertCoords(glm::vec2(241, 979)), true, false);
    _jetpackEmpty.Init("res/Images/HUD/jetpack_paliwo.png", ConvertCoords(glm::vec2(1719, 949)), ConvertCoords(glm::vec2(1891, 316)), true, false);
    _depthMeterBackground.Init("res/Images/HUD/depth0.png", ConvertCoords(glm::vec2(1662, 1051)), ConvertCoords(glm::vec2(1891, 967)), true, false);
    _waveTimerGreen.Init("res/Images/WaveTimer/zegar_zielony.png", ConvertCoords(glm::vec2(28, 234)), ConvertCoords(glm::vec2(219, 43)), true, false);
    _waveTimerRed.Init("res/Images/WaveTimer/zegar_czerwony.png", ConvertCoords(glm::vec2(28, 234)), ConvertCoords(glm::vec2(219, 43)), true, false);
    _waveArrowGreen.Init("res/Images/WaveTimer/strzalka_zielona.png", ConvertCoords(glm::vec2(124, 139)), 90, true, true);
    _waveArrowRed.Init("res/Images/WaveTimer/strzalka_czerwona.png", ConvertCoords(glm::vec2(124, 139)), 90, true, true);
    testowy.Init("res/Images/HUD/testowy.png", glm::vec2(-50, -50), glm::vec2(50, 50), true, false);

    for(int i = 0; i <= 20; i++){
        shared_ptr<ImageRenderer> hp = make_shared<ImageRenderer>();
        std::string path = "res/Images/HUD/BaseHP/Border/bar_" + std::to_string(5*i) + ".png";
        hp->Init(path.c_str(), ConvertCoords(glm::vec2(1689, 240)), ConvertCoords(glm::vec2(1891, 37)), true, false);
        _baseHPImages.push_back(hp);
    }

    for(int i = 0; i <= 3; i++){
        shared_ptr<ImageRenderer> hp = make_shared<ImageRenderer>();
        std::string path = "res/Images/HUD/BaseHP/InsideImages/inside" + std::to_string(i) + ".png";
        hp->Init(path.c_str(), ConvertCoords(glm::vec2(1711, 217)), ConvertCoords(glm::vec2(1869, 59)), true, false);
        _baseInsideImages.push_back(hp);
    }

    _playerNode = NODESMANAGER.getNodeByName("player");
}

glm::vec3 HUDMain::interpolateColor(float percentFuel) {
    glm::vec3 color;

    percentFuel = glm::clamp(percentFuel, 0.0f, 100.0f);

    if (percentFuel <= 50.0f) {
        color.r = 1.0f;
        color.g = percentFuel / 50.0f;
        color.b = 0.0f;
    } else {
        color.r = (100.0f - percentFuel) / 50.0f;
        color.g = 1.0f;
        color.b = 0.0f;
    }

    return color;
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
    if(_shouldShowHP){
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

        if (percentHP <= 0) {
            _baseHPImages[0]->Render();
        } else if (percentHP == 100) {
            _baseHPImages[20]->Render();
        } else {
            int index = (percentHP - 1) / 5;
            _baseHPImages[index + 1]->Render();
        }
    }
    if(_shouldShowCrosshair){
        _crosshairImage.Render();
    }

    //materials
    if(_shouldShowMaterials){
        _materialsBackground.Render();
        _plasticImage.Render();
        _metalImage.Render();
        TEXTRENDERER.RenderText(to_string(GAMEMANAGER._plastic), -0.8833333, -0.8888889, 0.35f, glm::vec3(1.0f, 1.0f, 1.0f));
        TEXTRENDERER.RenderText(to_string(GAMEMANAGER._metal), -0.7395833, -0.8888889, 0.35f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    /*
    //jetpack
    leftDownCorner = glm::vec3(0.8875, -0.7407407, 0.0);
    rightTopCorner = glm::vec3(0.963, 0.3981481, 0.0);
    float percentFuel = (_playerNode->GetComponent<PlayerController>()->_jetpackFuel/_playerNode->GetComponent<PlayerController>()->_maxJetpackFuel) * 100;
    float y = (percentFuel - 0.0f) * (rightTopCorner.y - (leftDownCorner.y)) / (100.0f - 0.0f) + (leftDownCorner.y);
    */

    if(_shouldShowFuel){
        _jetpackEmpty.Render();
    }


    //depthMeter
    if(_shouldShowDepth){
        _depthMeterBackground.Render();
        TEXTRENDERER.RenderText(to_string(-(int)std::floor(GAMEMANAGER._groundLevel - _playerNode->GetTransform()->GetPosition().y)), 0.82, -0.8833333, 0.35f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    //timer
    if(_shouldShowPhaseInfo){
        WaveTimerGUIManager();

        //TEXTRENDERER.RenderText("TTN: " + to_string(GAMEMANAGER.phaseTime - GAMEMANAGER.currentTime), -0.97f, 0.88f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

void HUDMain::WaveTimerGUIManager() {
    auto phaseTime = GAMEMANAGER.phaseTime;
    auto currentTime = GAMEMANAGER.currentTime;
    auto currentPhase = GAMEMANAGER.currentPhase;

    if (phaseTime == 0) {
        phaseTime = 0.001;
    }

    float proportion = currentTime / phaseTime;
    float degrees = -((proportion * 360.0f) - 90.0f);

    degrees = round(degrees);

    if (currentPhase == 0) {
        _waveTimerGreen.Render();
        _waveArrowGreen.Render();
        if (_clockTimer < _clockInterval) {
            _clockTimer += TIME.GetDeltaTime();
        } else {
            _waveArrowGreen.SetRotationAngle(degrees);
            _waveArrowRed.SetRotationAngle(degrees);
            _clockTimer = 0;
        }
    } else if (currentPhase == 1) {
        _waveTimerRed.Render();
        _waveArrowRed.Render();
        if (_clockTimer < _clockInterval) {
            _clockTimer += TIME.GetDeltaTime();
        } else {
            _waveArrowRed.SetRotationAngle(degrees);
            _waveArrowGreen.SetRotationAngle(degrees);
            _clockTimer = 0;
        }
    }
}