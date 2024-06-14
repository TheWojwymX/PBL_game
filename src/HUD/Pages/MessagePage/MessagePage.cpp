#include "MessagePage.h"

glm::vec2 MessagePage::ConvertCoords(const glm::vec2& coords) {
    float x = (coords.x / 1920.0f) * 100.0f - 50.0f;
    float y = (1.0f - (coords.y / 1080.0f)) * 100.0f - 50.0f;
    return glm::vec2(x, y);
}

void MessagePage::Init() {
    _backgroundImage.Init("res/Images/HUD/tutorial_window.png", ConvertCoords(glm::vec2(343, 1051)), ConvertCoords(glm::vec2(1633, 967)), true, false);
    Page::Init();
}

void MessagePage::Update() {
    if(_shouldRender){
        Page::Update();
        _backgroundImage.Render();
        TEXTRENDERER.RenderTextCentered(_actualText, 0.0f, -0.865, 0.4f, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}
