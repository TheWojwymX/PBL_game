//
// Created by Jacek on 09.05.2024.
//

#include "Page.h"

void Page::Init() {
    _backgroundImage.Init(_backgroundImagePath, _backgroundVertices, true, false);
}

void Page::Update() {
    _backgroundImage.UpdateImage();
}

void Page::SetVertices(const array<float, 32> &vertices) {
    _backgroundVertices = vertices;
}

void Page::SetBackgroundImagePath(const char *path) {
    _backgroundImagePath = path;
}
