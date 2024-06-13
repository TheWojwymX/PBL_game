#pragma once

#include "HUDMain.h"

HUDMain& HUDMain::getInstance() {
    static HUDMain instance;
    return instance;
}

void HUDMain::Init() {

    //text
    TEXTRENDERER.Init();

    _crosshairImage.Init("res/Images/crosshair041.png", glm::vec3(0, 0, 0), true, false);
    _materialsBackground.Init("res/Images/HUD/materials_background.png", glm::vec3(-45,-47.0,0), true, false);
    _plasticImage.Init("res/Images/HUD/plastic_icon.png", glm::vec3(-47, -47.0, 0), true, false);
    _metalImage.Init("res/Images/HUD/metal_icon.png", glm::vec3(-43, -47.0, 0), true, false);
    _jetpackEmpty.Init("res/Images/HUD/jetpack_paliwo.png", glm::vec3(45, -25, 0), true, false);
    _jetpackBar.Init("res/Images/HUD/jetpackBar.png", glm::vec3(45, -25, 0), false, false);
    _depthMeterBackground.Init("res/Images/HUD/depth0.png", glm::vec3(47, 30, 0), true, false);
    _waveTimerGreen.Init("res/Images/WaveTimer/zegar_zielony.png", glm::vec3(-47, 47, 0), true, false);
    _waveArrowRed.Init("res/Images/WaveTimer/strzalka_czerwona.png", glm::vec3(-49,49,0),true, true);

    
    for(int i = 0; i <= 20; i++){
        shared_ptr<ImageRenderer> hp = make_shared<ImageRenderer>();
        std::string path = "res/Images/HUD/BaseHP/Border/bar_" + std::to_string(5*i) + ".png";
        hp->Init(path.c_str(), glm::vec3(47,47,0), true, true);
        _baseHPImages.push_back(hp);
    }

    for(int i = 0; i <= 3; i++){
        shared_ptr<ImageRenderer> hp = make_shared<ImageRenderer>();
        std::string path = "res/Images/HUD/BaseHP/InsideImages/inside" + std::to_string(i) + ".png";
        hp->Init(path.c_str(), glm::vec3(47, 47, 0), true, true);
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
        testowy.UpdateImage();
    }

    glm::vec leftDownCorner = glm::vec3(0.7614583, 0.5564815, 0.0);
    glm::vec rightTopCorner = glm::vec3(0.9723958, 0.9314815, 0.0);
    std::array<float, 32> verticesHPBorder{
            // positions          // colors           // texture coords
            rightTopCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            rightTopCorner.x, rightTopCorner.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            leftDownCorner.x, rightTopCorner.y, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            leftDownCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    leftDownCorner = glm::vec3(0.78, 0.58, 0.0);
    rightTopCorner = glm::vec3(0.959, 0.899, 0.0);
    std::array<float, 32> verticesHPInside{
            // positions          // colors           // texture coords
            rightTopCorner.x,  leftDownCorner.y, rightTopCorner.z,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            rightTopCorner.x, rightTopCorner.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            leftDownCorner.x, rightTopCorner.y, leftDownCorner.z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            leftDownCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    float actualDomeHP = DOMEMANAGER.GetDomeHP();
    float maxHP = DOMEMANAGER.GetDomeMaxHP();
    float percentHP = actualDomeHP / maxHP * 100;

    //std::cout << actualDomeHP << "   " << maxHP << "   " << percentHP << "   " << (actualDomeHP/maxHP) * 100 << std::endl;
    if(_shouldShowHP){
        if(percentHP <= 0){
            _baseInsideImages[3]->UpdateImage(&verticesHPInside);
        }
        else if(percentHP < 33){
            _baseInsideImages[2]->UpdateImage(&verticesHPInside);
        }
        else if(percentHP < 66){
            _baseInsideImages[1]->UpdateImage(&verticesHPInside);
        }
        else{
            _baseInsideImages[0]->UpdateImage(&verticesHPInside);
        }

        if (percentHP <= 0) {
            _baseHPImages[0]->UpdateImage(&verticesHPBorder);
        } else if (percentHP == 100) {
            _baseHPImages[20]->UpdateImage(&verticesHPBorder);
        } else {
            int index = (percentHP - 1) / 5;
            _baseHPImages[index + 1]->UpdateImage(&verticesHPBorder);
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
        TEXTRENDERER.RenderText(to_string(GAMEMANAGER._metal), -0.8833333, -0.8888889, 0.35f, glm::vec3(1.0f, 1.0f, 1.0f));
        TEXTRENDERER.RenderText(to_string(GAMEMANAGER._plastic), -0.7395833, -0.8888889, 0.35f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    //jetpack
    leftDownCorner = glm::vec3(0.8875, -0.7407407, 0.0);
    rightTopCorner = glm::vec3(0.963, 0.3981481, 0.0);
    float percentFuel = (_playerNode->GetComponent<PlayerController>()->_jetpackFuel/_playerNode->GetComponent<PlayerController>()->_maxJetpackFuel) * 100;
    float y = (percentFuel - 0.0f) * (rightTopCorner.y - (leftDownCorner.y)) / (100.0f - 0.0f) + (leftDownCorner.y);

    std::array<float, 32> verticesJetpackBar{
            // positions          // colors           // texture coords
            rightTopCorner.x,  leftDownCorner.y, rightTopCorner.z,   1.0f, 0.0f, 0.0f,   1.0f, 0.0f, // bottom right
            rightTopCorner.x,  y, 0.0f,                             0.0f, 1.0f, 0.0f,   1.0f, 1.0f, // top right
            leftDownCorner.x,  y, leftDownCorner.z,                 0.0f, 0.0f, 1.0f,   0.0f, 1.0f, // top left
            leftDownCorner.x,  leftDownCorner.y, 0.0f,              1.0f, 1.0f, 0.0f,   1.0f, 0.0f  // bottom left
    };

    if(_shouldShowFuel){
        glm::vec3 color = interpolateColor(percentFuel);
        _jetpackBar.UpdateImage(&verticesJetpackBar, color);

        _jetpackEmpty.Render();
    }

    //depthMeter
    if(_shouldShowDepth){
        _depthMeterBackground.Render();
        TEXTRENDERER.RenderText(to_string(-(int)std::floor(GAMEMANAGER._groundLevel - _playerNode->GetTransform()->GetPosition().y)), 0.82, -0.8833333, 0.35f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    //timer
    if(_shouldShowPhaseInfo){

        //Timer
        _waveTimerGreen.Render();

        _waveArrowRed.AddRotation(15.0f * TIME.GetDeltaTime());
        _waveArrowRed.Render();

/*        TEXTRENDERER.RenderText("TTN: " + to_string(GAMEMANAGER.phaseTime - GAMEMANAGER.currentTime), -0.97f, 0.88f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

        string nextPhaseName;
        if(GAMEMANAGER.currentPhase == 0){
            nextPhaseName = "Setup";
        }
        else if(GAMEMANAGER.currentPhase == 1){
            nextPhaseName = "Enemy attack";
        }
        else if(GAMEMANAGER.currentPhase == 2){
            nextPhaseName = "Mining";
        }

        TEXTRENDERER.RenderText("Next phase: " + nextPhaseName, -0.97f, 0.75f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));*/
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}