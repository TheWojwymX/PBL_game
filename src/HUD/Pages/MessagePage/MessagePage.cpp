//
// Created by Jacek on 25.05.2024.
//

#include "MessagePage.h"

void MessagePage::Init() {

    SetVertices(_backgroundVertices);

    _generalImage.Init(_generalImagePath, _generalImageVertices, true, false);

    _continueDialogButton.Init();

    Page::Init();
}

void MessagePage::Update() {
    if(_shouldRender){
        Page::Update();
        _generalImage.UpdateImage();
        _continueDialogButton.Update();
        TEXTRENDERER.RenderText(_actualText, -0.5f,  -0.3f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}

void MessagePage::SetVertices(const array<float, 32> &vertices) {
    Page::SetVertices(vertices);
}
