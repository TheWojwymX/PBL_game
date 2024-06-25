//
// Created by Jacek on 24.06.2024.
//

#include "ControlsPage.h"
#include "HUD/PageManager.h"

void ControlsPage::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetCorners(glm::vec2(0, 0), glm::vec2(0, 0));
    SetupButtons();
    _backButton->Init();
    _buttonsBackground.Init("res/Images/MainMenuSettings/settings_control.png", CoordsConverter::ConvertCoords(glm::vec2(463, 779)), CoordsConverter::ConvertCoords(glm::vec2(1456, 370)), true, false);
    _logo.Init("res/Images/logo.png", CoordsConverter::ConvertCoords(glm::vec2(696, 224)), CoordsConverter::ConvertCoords(glm::vec2(1235, 118)), true, false);
    _backgroundMainMenu.Init(_backgroundMainMenuImagePath, glm::vec2(-50, -50), glm::vec2(50, 50), true, false);
    Page::Init();
}

void ControlsPage::Update() {
    if (_shouldRender) {
        if(_mode == 0){
            _backgroundMainMenu.Render();
        }

        Page::Update();
        _buttonsBackground.Render();
        _backButton->Update();
        _logo.Render();
    }
}

void ControlsPage::SetupButtons() {
    _backButton = std::make_shared<Button>("res/Images/Button/button_back.png",
                                           "res/Images/Button/button_back_hover.png",
                                           "res/Images/Button/button_back_clicked.png",
                                           CoordsConverter::ConvertCoords(glm::vec2(1249, 748)),
                                           CoordsConverter::ConvertCoords(glm::vec2(1428, 697)),
                                           [this]() {
                                               if (_mode == 0) {
                                                   PAGEMANAGER.GoToSettings(0);
                                               } else {
                                                   PAGEMANAGER.GoToSettings(1);
                                               }
                                           });
}