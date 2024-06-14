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
    void SetCorners(glm::vec2 downLeftCorner, glm::vec2 topRightCorner);

private:
    glm::vec2 _downLeftCorner;
    glm::vec2 _topRightCorner;
};
