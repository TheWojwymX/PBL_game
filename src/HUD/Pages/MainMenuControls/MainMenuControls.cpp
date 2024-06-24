//
// Created by Jacek on 24.06.2024.
//

#include "MainMenuControls.h"
#include "HUD/PageManager.h"

void MainMenuControls::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetCorners(glm::vec2(-50, -50), glm::vec2(50, 50));
    SetupButtons();
    _backButton->Init();
    _buttonsBackground.Init("res/Images/MainMenuSettings/settings_control.png", CoordsConverter::ConvertCoords(glm::vec2(463, 836)), CoordsConverter::ConvertCoords(glm::vec2(1456, 370)), true, false);
    _logo.Init("res/Images/logo.png", CoordsConverter::ConvertCoords(glm::vec2(696, 224)), CoordsConverter::ConvertCoords(glm::vec2(1235, 118)), true, false);
    Page::Init();
}

void MainMenuControls::Update() {
    if (_shouldRender) {
        Page::Update();
        _buttonsBackground.Render();
        _backButton->Update();
        _logo.Render();
    }
}

void MainMenuControls::SetupButtons() {
    _backButton = std::make_shared<Button>("res/Images/Button/button_back.png",
                                           "res/Images/Button/button_back_hover.png",
                                           "res/Images/Button/button_back_clicked.png",
                                           CoordsConverter::ConvertCoords(glm::vec2(1249, 805)),
                                           CoordsConverter::ConvertCoords(glm::vec2(1428, 754)),
                                           [this]() {
                                               PAGEMANAGER.GoToSettingsMainMenu();
                                           });
}