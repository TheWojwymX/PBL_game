//
// Created by Jacek on 09.05.2024.
//

#ifndef SANDBOX_PAGE_H
#define SANDBOX_PAGE_H
#include <array>
#include "HUD/Image/ImageRenderer.h"

class Page {
public:

    virtual void Init();
    virtual void Update();

    std::array<float, 32> _backgroundVertices{
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    const char *_backgroundImagePath = "res/Images/PageBackground.png";

    ImageRenderer _backgroundImage;

    bool _shouldRender = false;

    virtual void SetVertices(const array<float, 32> &vertices);
};


#endif //SANDBOX_PAGE_H
