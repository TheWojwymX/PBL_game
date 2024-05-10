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
