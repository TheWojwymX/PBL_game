#include "Page.h"

void Page::Init() {
    _backgroundImage.Init(_backgroundImagePath, glm::vec3(0,0,0), true, false);
}

void Page::Update() {
    _backgroundImage.Render();
}

void Page::SetBackgroundImagePath(const char *path) {
    _backgroundImagePath = path;
}
