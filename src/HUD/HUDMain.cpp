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
            1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -1.0f,  1.0, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    _hpEmptyImage.Init("../../res/Images/hp_empty.png", verticesHpEmpty, true, false);
    _hpFullImage.Init("../../res/Images/hp_full.png", verticesHpFull, true, true);
    _crosshairImage.Init("../../res/Images/crosshair041.png", verticesCrosshair, true, false);
    _animatedImage.Init("../../res/Images/5margin.png", vertices5, true, true);
}

int xx = 5;
int yy = 5;
int spriteSheetWidth = 1330;
int spriteSheetHeight = 532;
int spriteWidth = 256;
int spriteHeight = 256;
float test = 0;

void HUDMain::Update() {

    static float lastTime = 0.0f;
    float currentTime = glfwGetTime();
    float interval = 0.05f;

    if (currentTime - lastTime >= interval) {

        ///animation
/*        if((int)test % 5 == 0 && test != 0){
            xx = 5;
            if(yy == 5){
                yy = 271;
            }
            else{
                yy = 5;
            }
        }
        else{
            xx = xx + spriteWidth + 10;
        }*/
        ///

        test++;
        lastTime = currentTime;
    }

    //images
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    ///animated image
/*    float texCoordLeft = xx / static_cast<float>(spriteSheetWidth);
    float texCoordRight = (xx + spriteWidth) / static_cast<float>(spriteSheetWidth);
    float texCoordTop = yy / static_cast<float>(spriteSheetHeight);
    float texCoordBottom = (yy + spriteHeight) / static_cast<float>(spriteSheetHeight);

    std::array<float, 32> vertices5{
            // positions          // colors           // texture coords
            1.0f,  1.0f, 0.0f,   1.0f, 0.0f, 0.0f,   texCoordRight, texCoordTop, // top right
            1.0f, -1.0f, 0.0f,   0.0f, 1.0f, 0.0f,   texCoordRight, texCoordBottom, // bottom right
            -1.0f, -1.0f, 0.0f,   0.0f, 0.0f, 1.0f,   texCoordLeft, texCoordBottom, // bottom left
            -1.0f,  1.0, 0.0f,   1.0f, 1.0f, 0.0f,   texCoordLeft, texCoordTop  // top left
    };

    _animatedImage.UpdateImage(&vertices5);*/
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
