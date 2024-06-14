#include "Page.h"

void Page::Init() {
    _backgroundImage.Init(_backgroundImagePath, _downLeftCorner, _topRightCorner, true, false);
}

void Page::Update() {
    _backgroundImage.Render();
}

void Page::SetBackgroundImagePath(const char *path) {
    _backgroundImagePath = path;
}

void Page::SetCorners(glm::vec2 downLeftCorner, glm::vec2 topRightCorner)
{
    _downLeftCorner = downLeftCorner;
    _topRightCorner = topRightCorner;
}
