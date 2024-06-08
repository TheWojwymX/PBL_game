//
// Created by Jacek on 08.06.2024.
//

#ifndef SANDBOX_PDAPAGE_H
#define SANDBOX_PDAPAGE_H

#include "HUD/Pages/Page.h"

class PDAPage : public Page {

public:

    std::array<float, 32> _backgroundVertices{
            // positions          // colors           // texture coords
            0.0f,  0.0f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.0f, -0.0f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.0f, -0.0f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.0f,  0.0f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    glm::vec3 leftDownCorner = glm::vec3(-0.6, -0.4, 0.0);
    glm::vec3 rightTopCorner = glm::vec3(0.6, 0.4, 0.0);
    std::array<float, 32> verticesPDAMenu{
            // positions          // colors           // texture coords
            rightTopCorner.x,  leftDownCorner.y, rightTopCorner.z,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            rightTopCorner.x, rightTopCorner.y, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            leftDownCorner.x, rightTopCorner.y, leftDownCorner.z,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            leftDownCorner.x,  leftDownCorner.y, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    ImageRenderer _PDAMenu;

    void Init() override;

    void Update() override;

    void SetVertices(const array<float, 32> &vertices) override;

    void DisplayPDAPage();

    void HidePDAPage();
};


#endif //SANDBOX_PDAPAGE_H
