#include "MessagePage.h"

void MessagePage::Init() {
    _generalImage.Init(_generalImagePath, glm::vec2(0,-40),0,true, false);
    Page::Init();
}

void MessagePage::Update() {
    if(_shouldRender){
        Page::Update();
        _generalImage.UpdateImage();
        TEXTRENDERER.RenderText(_actualText, -0.28f,  -0.7f, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}
