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
    _mainMenuButton->Init();
    _logo.Init("res/Images/logo.png", CoordsConverter::ConvertCoords(glm::vec2(607, 149)), CoordsConverter::ConvertCoords(glm::vec2(1231, 36)), true, false);
    _gameOver.Init("res/Images/gameOver.png", CoordsConverter::ConvertCoords(glm::vec2(369, 413)), CoordsConverter::ConvertCoords(glm::vec2(1541, 227)), true, false);
    Page::Init();
}

void RestartPage::Update() {
    if (_shouldRender) {
        Page::Update();
        _restartButton->Update();
        _mainMenuButton->Update();
        _gameOver.Render();
        _logo.Render();
    }
}

void RestartPage::SetupButtons() {
    _restartButton = std::make_shared<Button>("res/Images/Button/restart.png",
                                            "res/Images/Button/restart_hover.png",
                                            "res/Images/Button/restart_clicked.png",
                                            CoordsConverter::ConvertCoords(glm::vec2(785, 583)),
                                            CoordsConverter::ConvertCoords(glm::vec2(1142, 482)),
                                            [this]() {
                                                GAMEMANAGER.RestartGame();
                                                GAMEMANAGER._clickedStart = true;
                                            });

    _mainMenuButton = std::make_shared<Button>("res/Images/Button/mainmenu.png",
                                              "res/Images/Button/mainmenu_hover.png",
                                              "res/Images/Button/mainmenu_clicked.png",
                                              CoordsConverter::ConvertCoords(glm::vec2(785, 720)),
                                              CoordsConverter::ConvertCoords(glm::vec2(1142, 619)),
                                              [this]() {
                                                  GAMEMANAGER.GoToMainMenu();
                                                  GAMEMANAGER._clickedStart = false;
                                              });
}