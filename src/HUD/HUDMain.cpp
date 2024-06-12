//
// Created by Jacek on 03.04.2024.
//

#include "HUDMain.h"

HUDMain& HUDMain::getInstance() {
    static HUDMain instance;
    return instance;
}

void HUDMain::Init() {

    //text
    TEXTRENDERER.Init();

    //images
    std::array<float, 32> verticesCrosshair{
            // positions          // colors           // texture coords
            0.03f,  0.05f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.03f, -0.05f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.03f, -0.05f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.03f,  0.05f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    glm::vec3 leftDownCorner = glm::vec3(-0.98, -0.98, 0.0);
    glm::vec3 rightTopCorner = glm::vec3(-0.80, -0.75, 0.0);
    std::array<float, 32> verticesPlastic{
            // positions          // colors           // texture coords
            rightTopCorner.x,  leftDownCorner.y, rightTopCorner.z,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            rightTopCorner.x, rightTopCorner.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            leftDownCorner.x, rightTopCorner.y, leftDownCorner.z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            leftDownCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    leftDownCorner = glm::vec3(-0.58, -0.98, 0.0);
    rightTopCorner = glm::vec3(-0.40, -0.75, 0.0);
    std::array<float, 32> verticesMetal{
            // positions          // colors           // texture coords
            rightTopCorner.x,  leftDownCorner.y, rightTopCorner.z,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            rightTopCorner.x, rightTopCorner.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            leftDownCorner.x, rightTopCorner.y, leftDownCorner.z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            leftDownCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    leftDownCorner = glm::vec3(0.78, -0.95, 0.0);
    rightTopCorner = glm::vec3(0.95, 0.40, 0.0);
    std::array<float, 32> verticesJetpackEmpty{
            // positions          // colors           // texture coords
            rightTopCorner.x,  leftDownCorner.y, rightTopCorner.z,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            rightTopCorner.x, rightTopCorner.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            leftDownCorner.x, rightTopCorner.y, leftDownCorner.z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            leftDownCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    std::array<float, 32> hpVertices{
            // positions          // colors           // texture coords
            hpTopRight.x,  hpTopRight.y, 0.0f,        1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            hpBottomRight.x, hpBottomRight.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            hpBottomLeft.x, hpBottomLeft.y, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            hpTopLeft.x, hpTopLeft.y, 0.0f,           1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    leftDownCorner = glm::vec3(0.83, -0.8, 0.0);
    rightTopCorner = glm::vec3(0.95, 0.40, 0.0);
    std::array<float, 32> verticesJetpackBar{
            // positions          // colors           // texture coords
            rightTopCorner.x,  leftDownCorner.y, rightTopCorner.z,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            rightTopCorner.x, rightTopCorner.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            leftDownCorner.x, rightTopCorner.y, leftDownCorner.z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            leftDownCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    leftDownCorner = glm::vec3(-1, 0.4, 0.0);
    rightTopCorner = glm::vec3(-0.5, 1, 0.0);
    std::array<float, 32> verticesTimerBar{
            // positions          // colors           // texture coords
            rightTopCorner.x,  leftDownCorner.y, rightTopCorner.z,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            rightTopCorner.x, rightTopCorner.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            leftDownCorner.x, rightTopCorner.y, leftDownCorner.z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            leftDownCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    glm::vec3 middlePoint = glm::vec3((leftDownCorner.x + rightTopCorner.x) / 2.0f, (leftDownCorner.y + rightTopCorner.y) / 2.0f, 0);
    leftDownCorner = glm::vec3(middlePoint.x, middlePoint.y - 0.05, 0);
    rightTopCorner = glm::vec3(middlePoint.x + 0.25, middlePoint.y + 0.05, 0);
    std::array<float, 32> verticesArrow{
            // positions          // colors           // texture coords
            rightTopCorner.x,  leftDownCorner.y, rightTopCorner.z,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            rightTopCorner.x, rightTopCorner.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            leftDownCorner.x, rightTopCorner.y, leftDownCorner.z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            leftDownCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    _waveArrowRed._rotationAngle = 0;
    _waveArrowRed.Init("res/Images/WaveTimer/strzalka_czerwona.png", verticesArrow, true, false);

    _waveTimerGreen.Init("res/Images/WaveTimer/zegar_zielony.png", verticesTimerBar, true, false);
    _crosshairImage.Init("../../res/Images/crosshair041.png", verticesCrosshair, true, false);
    _plasticImage.Init("../../res/Images/Icons/plasticIcon.png", verticesPlastic, true, false);
    _metalImage.Init("../../res/Images/Icons/metalIcon.png", verticesMetal, true, false);
    _jetpackEmpty.Init("../../res/Images/jetpackEmptyBar.png", verticesJetpackEmpty, true, false);
    _jetpackBar.Init("../../res/Images/jetpackBar.png", verticesJetpackBar, false, false);


    for(int i = 0; i < 17; i++){
        shared_ptr<ImageRenderer> hp = make_shared<ImageRenderer>();
        std::string path = "../../res/BaseHP/base_hp_" + std::to_string(i) + "_spritesheet.png";
        hp->Init(path.c_str(), hpVertices, true, true);
        _baseHPImages.push_back(hp);
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


    if(_hpSpriteTimer < _hpSpriteInterval){
        _hpSpriteTimer += TIME.GetDeltaTime();
    }else{
        ///animation
        switch (_hpSpriteCurrentFrame) {
            case 0:
                _hpTextureX = 0;
                _hpSpriteCurrentFrame++;
                break;
            case 1:
                _hpTextureX += _hpSpriteWidth;
                _hpSpriteCurrentFrame++;
                break;
            case 2:
                _hpTextureX += _hpSpriteWidth;
                _hpSpriteCurrentFrame++;
                break;
            case 3:
                _hpTextureX += _hpSpriteWidth;
                _hpSpriteCurrentFrame = 0;
                break;
        }
        _hpSpriteTimer = 0;
    }

    ///animated image
    float texCoordLeft = _hpTextureX / static_cast<float>(_hpSpriteSheetWidth);
    float texCoordRight = (_hpTextureX + _hpSpriteWidth) / static_cast<float>(_hpSpriteSheetWidth);
    float texCoordTop = _hpTextureY / static_cast<float>(_hpSpriteSheetHeight);
    float texCoordBottom = (_hpTextureY + _hpSpriteHeight) / static_cast<float>(_hpSpriteSheetHeight);

    std::array<float, 32> hpVertices{
            // positions          // colors         // texture coords
            hpTopRight.x,  hpTopRight.y, 0.0f,      1.0f, 0.0f, 0.0f,   texCoordRight, texCoordTop, // top right
            hpBottomRight.x, hpBottomRight.y, 0.0f, 0.0f, 1.0f, 0.0f,   texCoordRight, texCoordBottom, // bottom right
            hpBottomLeft.x, hpBottomLeft.y, 0.0f,   0.0f, 0.0f, 1.0f,   texCoordLeft, texCoordBottom, // bottom left
            hpTopLeft.x, hpTopLeft.y, 0.0f,         1.0f, 1.0f, 0.0f,   texCoordLeft, texCoordTop  // top left
    };

    int actualDomeHP = DOMEMANAGER.hp;
    int maxHP = DOMEMANAGER.maxHP;
    float percentHP = (static_cast<float>(actualDomeHP) / static_cast<float>(maxHP)) * 100;

    //std::cout << actualDomeHP << "   " << maxHP << "   " << percentHP << "   " << (actualDomeHP/maxHP) * 100 << std::endl;
    if(_shouldShowHP){
        if(percentHP > 0 && percentHP <= 6){
            _baseHPImages[0]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 6 && percentHP <= 12){
            _baseHPImages[1]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 12 && percentHP <= 19){
            _baseHPImages[2]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 19 && percentHP <= 25){
            _baseHPImages[3]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 25 && percentHP <= 31){
            _baseHPImages[4]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 31 && percentHP <= 37){
            _baseHPImages[5]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 37 && percentHP <= 44){
            _baseHPImages[6]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 44 && percentHP <= 50){
            _baseHPImages[7]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 50 && percentHP <= 56){
            _baseHPImages[8]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 56 && percentHP <= 62){
            _baseHPImages[9]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 62 && percentHP <= 69){
            _baseHPImages[10]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 69 && percentHP <= 75){
            _baseHPImages[11]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 75 && percentHP <= 81){
            _baseHPImages[12]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 81 && percentHP <= 87){
            _baseHPImages[13]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 87 && percentHP <= 93){
            _baseHPImages[14]->UpdateImage(&hpVertices);
        }
        else if(percentHP > 93 && percentHP < 100){
            _baseHPImages[15]->UpdateImage(&hpVertices);
        }
        else if(percentHP == 100){
            _baseHPImages[16]->UpdateImage(&hpVertices);
        }
    }

    if(_shouldShowCrosshair){
        _crosshairImage.UpdateImage();
    }

    if(_shouldShowMaterials){
        _plasticImage.UpdateImage();
        _metalImage.UpdateImage();
    }

    //jetpack
    glm::vec3 leftDownCorner = glm::vec3(0.83, -0.8, 0.0);
    glm::vec3 rightTopCorner = glm::vec3(0.95, 0.40, 0.0);
    float percentFuel = (_playerNode->GetComponent<PlayerController>()->_jetpackFuel/_playerNode->GetComponent<PlayerController>()->_maxJetpackFuel) * 100;
    float y = (percentFuel - 0.0f) * (0.40f - (-0.8f)) / (100.0f - 0.0f) + (-0.8f);

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

        _jetpackEmpty.UpdateImage();
    }

    //Timer
    _waveTimerGreen.UpdateImage();

    _waveArrowRed._rotationAngle = _waveArrowRed._rotationAngle - 0.1;
    _waveArrowRed.UpdateImage();

    //text
    if(_shouldShowMaterials){
        TEXTRENDERER.RenderText(to_string(GAMEMANAGER._metal), -0.38f, -0.92f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

        TEXTRENDERER.RenderText(to_string(GAMEMANAGER._plastic), -0.78f, -0.92f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    if(_shouldShowPhaseInfo){
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

    if(_shouldShowDepth){
        TEXTRENDERER.RenderText("Y: " + to_string(-(int)std::floor(GAMEMANAGER._groundLevel - _playerNode->GetTransform()->GetPosition().y)), 0.60, 0.35f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));
    }

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}