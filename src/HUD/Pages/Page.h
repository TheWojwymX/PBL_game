#pragma once

#include <array>
#include "HUD/Image/ImageRenderer.h"

class Page {
public:

    virtual void Init();
    virtual void Update();

    const char *_backgroundImagePath = "res/Images/PageBackground.png";

    ImageRenderer _backgroundImage;

    bool _shouldRender = false;

    virtual void SetBackgroundImagePath(const char * path);
};
