#include "MessagePage.h"
#include "HUD/CoordsConverter.h"

void MessagePage::Init() {
    _backgroundImage.Init("res/Images/HUD/tutorial_window.png", CoordsConverter::ConvertCoords(glm::vec2(343, 1051)), CoordsConverter::ConvertCoords(glm::vec2(1633, 967)), true, false);
    Page::Init();
}

void MessagePage::Update() {
    if(_shouldRender){
        Page::Update();
        _backgroundImage.Render();
        TEXTRENDERER.RenderTextCentered(_actualText, 0.0f, -0.865, 0.4f, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}
