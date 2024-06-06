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
    std::array<float, 32> verticesHpEmpty{
            // positions          // colors           // texture coords
            0.8f,  0.93f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.8f, 0.88f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.8f, 0.88f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.8f,  0.93f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    std::array<float, 32> verticesHpFull{
            // positions          // colors           // texture coords
            0.8f,  0.93f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.8f, 0.88f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.8f, 0.88f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.8f,  0.93f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    std::array<float, 32> verticesCrosshair{
            // positions          // colors           // texture coords
            0.03f,  0.05f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.03f, -0.05f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.03f, -0.05f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.03f,  0.05f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    std::array<float, 32> hpVertices{
            // positions          // colors           // texture coords
            hpTopRight.x,  hpTopRight.y, 0.0f,        1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            hpBottomRight.x, hpBottomRight.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            hpBottomLeft.x, hpBottomLeft.y, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            hpTopLeft.x, hpTopLeft.y, 0.0f,           1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    //_hpEmptyImage.Init("../../res/Images/hp_empty.png", verticesHpEmpty, true, false);
    //_hpFullImage.Init("../../res/Images/hp_full.png", verticesHpFull, true, true);
    _crosshairImage.Init("../../res/Images/crosshair041.png", verticesCrosshair, true, false);
    //_animatedImage.Init("../../res/Images/5margin.png", vertices5, true, true);

    for(int i = 0; i < 17; i++){
        shared_ptr<ImageRenderer> hp = make_shared<ImageRenderer>();
        std::string path = "../../res/BaseHP/base_hp_" + std::to_string(i) + "_spritesheet.png";
        hp->Init(path.c_str(), hpVertices, true, true);
        _baseHPImages.push_back(hp);
    }

    _playerNode = NODESMANAGER.getNodeByName("player");
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
    ///
    _crosshairImage.UpdateImage();

    //text
    TEXTRENDERER.RenderText("P: " + to_string(GAMEMANAGER._plastic) + "  M: " + to_string(GAMEMANAGER._metal), -0.97f, -0.95f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

    TEXTRENDERER.RenderText("TTN: " + to_string(GAMEMANAGER.phaseTime - GAMEMANAGER.currentTime), -0.97f, 0.88f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

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

    TEXTRENDERER.RenderText("Next phase: " + nextPhaseName, -0.97f, 0.75f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

    TEXTRENDERER.RenderText("Fuel: " + to_string((_playerNode->GetComponent<PlayerController>()->
            _jetpackFuel/_playerNode->GetComponent<PlayerController>()->_maxJetpackFuel) * 100), 0.30, -0.95f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

    TEXTRENDERER.RenderText("Y: " + to_string((int)std::floor(GAMEMANAGER._groundLevel - _playerNode->GetTransform()->GetPosition().y)), 0.60, 0.35f, 1.0f, glm::vec3(1.0f, 1.0f, 1.0f));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}
