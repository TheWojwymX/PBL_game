//
// Created by Jacek on 25.05.2024.
//

#include "DialogPage.h"

void DialogPage::Init() {

    SetVertices(_backgroundVertices);

    _generalImage.Init(_generalImagePath, _generalImageVertices, true, false);

/*    _upgradeDomeHP.Init();*/

    Page::Init();
}

void DialogPage::Update() {
    if(_shouldRender){
        Page::Update();
        _generalImage.UpdateImage();
/*        _upgradeDomeHP.Update();*/
        TEXTRENDERER.RenderText(_actualText, -0.5f,  -0.3f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}

void DialogPage::SetVertices(const array<float, 32> &vertices) {
    Page::SetVertices(vertices);
}
