//
// Created by Jacek on 03.04.2024.
//

#include "HUDMain.h"

float test;

void HUDMain::Init() {

    test = 0;

    //text
    TEXTRENDERER.init();

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

    hpEmptyImage.Init("../../res/Images/hp_empty.png", verticesHpEmpty, true, false);
    hpFullImage.Init("../../res/Images/hp_full.png", verticesHpFull, true, true);
    crosshairImage.Init("../../res/Images/crosshair041.png", verticesCrosshair, true, false);
}

void HUDMain::Update() {

    static float lastTime = 0.0f;
    float currentTime = glfwGetTime();
    float interval = 1.0f;

    if (currentTime - lastTime >= interval) {
        test++;
        lastTime = currentTime;
    }

    //text
    TEXTRENDERER.RenderText(*RESOURCEMANAGER.GetShaderByName("textShader"), to_string(test), 25.0f, 25.0f, 1.0f, glm::vec3(0.5, 0.8f, 0.2f));

    //images
    glDisable(GL_DEPTH_TEST);
    glDisable(GL_CULL_FACE);

    hpEmptyImage.UpdateImage();

    std::array<float, 32> verticesHpFull{
            // positions          // colors           // texture coords
            test/100,  0.93f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            test/100, 0.88f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.8f, 0.88f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.8f,  0.93f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    hpFullImage.UpdateImage(&verticesHpFull);
    crosshairImage.UpdateImage();

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);
}

HUDMain& HUDMain::getInstance() {
    static HUDMain instance;
    return instance;
}
