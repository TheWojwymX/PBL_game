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

    std::array<float, 32> vertices5{
            // positions          // colors           // texture coords
            hpTopRight.x,  hpTopRight.y, 0.0f,        1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            hpBottomRight.x, hpBottomRight.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            hpBottomLeft.x, hpBottomLeft.y, 0.0f,     0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            hpTopLeft.x, hpTopLeft.y, 0.0f,           1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    _hpEmptyImage.Init("../../res/Images/hp_empty.png", verticesHpEmpty, true, false);
    _hpFullImage.Init("../../res/Images/hp_full.png", verticesHpFull, true, true);
    _crosshairImage.Init("../../res/Images/crosshair041.png", verticesCrosshair, true, false);
    _animatedImage.Init("../../res/BaseHP/100hp.png", vertices5, true, true);
    //_animatedImage.Init("../../res/Images/5margin.png", vertices5, true, true);
}

int xx = 0;
int yy = 0;
int spriteSheetWidth = 256;
int spriteSheetHeight = 64;
int spriteWidth = 64;
int spriteHeight = 64;
float test = 0;
float testTimer = 0.0;
float interval = 0.3f;
int currentFrame = 0;

void HUDMain::Update() {

    //images
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    test++;

    if(testTimer < interval){
        testTimer += TIME.GetDeltaTime();
    }else{
        ///animation
        switch (currentFrame) {
            case 0:
                xx = 0;
                currentFrame++;
                break;
            case 1:
                xx += spriteWidth;
                currentFrame++;
                break;
            case 2:
                xx += spriteWidth;
                currentFrame++;
                break;
            case 3:
                xx += spriteWidth;
                currentFrame = 0;
                break;
        }
        testTimer = 0;
    }

    ///animated image
    float texCoordLeft = xx / static_cast<float>(spriteSheetWidth);
    float texCoordRight = (xx + spriteWidth) / static_cast<float>(spriteSheetWidth);
    float texCoordTop = yy / static_cast<float>(spriteSheetHeight);
    float texCoordBottom = (yy + spriteHeight) / static_cast<float>(spriteSheetHeight);

    std::array<float, 32> vertices5{
            // positions          // colors         // texture coords
            hpTopRight.x,  hpTopRight.y, 0.0f,      1.0f, 0.0f, 0.0f,   texCoordRight, texCoordTop, // top right
            hpBottomRight.x, hpBottomRight.y, 0.0f, 0.0f, 1.0f, 0.0f,   texCoordRight, texCoordBottom, // bottom right
            hpBottomLeft.x, hpBottomLeft.y, 0.0f,   0.0f, 0.0f, 1.0f,   texCoordLeft, texCoordBottom, // bottom left
            hpTopLeft.x, hpTopLeft.y, 0.0f,         1.0f, 1.0f, 0.0f,   texCoordLeft, texCoordTop  // top left
    };

    _animatedImage.UpdateImage(&vertices5);
    ///

    _hpEmptyImage.UpdateImage();

    std::array<float, 32> verticesHpFull{
            // positions          // colors           // texture coords
            test/100 - 0.8f,  0.93f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            test/100 - 0.8f, 0.88f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.8f, 0.88f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.8f,  0.93f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    _hpFullImage.UpdateImage(&verticesHpFull);
    _crosshairImage.UpdateImage();

    //text
    TEXTRENDERER.RenderText(to_string(test), -0.95f, -0.95f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}
