#include "PDAPage.h"
#include "HUD/PageManager.h"
#include "Turrets/TurretsManager.h"

void PDAPage::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetCorners(CoordsConverter::ConvertCoords(glm::vec2(462, 770)), CoordsConverter::ConvertCoords(glm::vec2(1457, 309)));
    TEXTRENDERER.Init();
    Page::Init();
}

void PDAPage::Update() {
    if(_shouldRender){
        Page::Update();

        //minigun
        glm::vec2 coords = CoordsConverter::ConvertTextCoords(glm::vec2(557, 745));
        TEXTRENDERER.RenderText(to_string(TURRETSMANAGER.GetTurretCost(MINIGUN)), coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

        //sniper
        coords = CoordsConverter::ConvertTextCoords(glm::vec2(864, 745));
        TEXTRENDERER.RenderText(to_string(TURRETSMANAGER.GetTurretCost(SNIPER)), coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

        //rifle
        coords = CoordsConverter::ConvertTextCoords(glm::vec2(1170, 745));
        TEXTRENDERER.RenderText(to_string(TURRETSMANAGER.GetTurretCost(RIFLE)), coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}

void PDAPage::DisplayPDAPage(){
    PAGEMANAGER._isInPage = true;
    _shouldRender = true;
}

void PDAPage::HidePDAPage(){
    PAGEMANAGER._isInPage = false;
    _shouldRender = false;
}