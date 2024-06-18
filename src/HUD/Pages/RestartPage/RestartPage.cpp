//
// Created by Jacek on 18.06.2024.
//

#include "RestartPage.h"

void RestartPage::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetCorners(glm::vec2(-50, -50), glm::vec2(50, 50));
    SetupButtons();
    _shouldRender = true;
    _restartButton->Init();
    _logo.Init("res/Images/logo.png", CoordsConverter::ConvertCoords(glm::vec2(1300, 140)), CoordsConverter::ConvertCoords(glm::vec2(1899, 22)), true, false);
    Page::Init();
}

void RestartPage::Update() {
    if (_shouldRender) {
        Page::Update();
        _restartButton->Update();
        _logo.Render();
        std::cout << "czy to sie wyswietla" << std::endl;
    }
}

void RestartPage::SetupButtons() {
    _restartButton = std::make_shared<Button>("res/Images/Button/start.png",
                                            "res/Images/Button/start_hover.png",
                                            "res/Images/Button/start_clicked.png",
                                            CoordsConverter::ConvertCoords(glm::vec2(1449, 258)),
                                            CoordsConverter::ConvertCoords(glm::vec2(1765, 170)),
                                            [this]() {
                                                GAMEMANAGER.RestartGame();
                                            });
}