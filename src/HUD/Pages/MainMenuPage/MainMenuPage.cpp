#include "MainMenuPage.h"

void MainMenuPage::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetCorners(glm::vec2(-50, -50), glm::vec2(50, 50));
    SetupButtons();
    _shouldRender = true;
    _startButton->Init();
    _settingsButton->Init();
    _creditsButton->Init();
    _quitButton->Init();
    _logo.Init("res/Images/logo.png", CoordsConverter::ConvertCoords(glm::vec2(1300, 140)), CoordsConverter::ConvertCoords(glm::vec2(1899, 22)), true, false);
    Page::Init();
}

void MainMenuPage::Update() {
    if (_shouldRender) {
        Page::Update();
        _startButton->Update();
        _settingsButton->Update();
        _creditsButton->Update();
        _quitButton->Update();
        _logo.Render();
    }
}

void MainMenuPage::SetupButtons() {
    _startButton = std::make_shared<Button>("res/Images/Button/start.png",
        "res/Images/Button/start_hover.png",
        "res/Images/Button/start_clicked.png",
        CoordsConverter::ConvertCoords(glm::vec2(1449, 258)),
        CoordsConverter::ConvertCoords(glm::vec2(1765, 170)),
        [this]() {
            GAMEMANAGER.StartGame();
        });

    _settingsButton = std::make_shared<Button>("res/Images/Button/settings.png",
        "res/Images/Button/settings_hover.png",
        "res/Images/Button/settings_clicked.png",
        CoordsConverter::ConvertCoords(glm::vec2(1449, 372)),
        CoordsConverter::ConvertCoords(glm::vec2(1765, 282)),
        [this]() {
            SettingsTest();
        });

    _creditsButton = std::make_shared<Button>("res/Images/Button/credits.png",
        "res/Images/Button/credits_hover.png",
        "res/Images/Button/credits_clicked.png",
       CoordsConverter::ConvertCoords(glm::vec2(1449, 487)),
       CoordsConverter::ConvertCoords(glm::vec2(1765, 397)),
        [this]() {
            CreditsButtonOnClick();
        });

    _quitButton = std::make_shared<Button>("res/Images/Button/quit.png",
        "res/Images/Button/quit_hover.png",
        "res/Images/Button/quit_clicked.png",
        CoordsConverter::ConvertCoords(glm::vec2(1449, 602)),
        CoordsConverter::ConvertCoords(glm::vec2(1765, 512)),
        [this]() {
            QuitButtonOnClick();
        });
}

void MainMenuPage::SettingsTest() {
    std::cout << "Settings Test" << std::endl;
}

void MainMenuPage::CreditsButtonOnClick() {
    std::cout << "Credits Button Clicked" << std::endl;
}

void MainMenuPage::QuitButtonOnClick() {
    glfwSetWindowShouldClose(GAMEMANAGER._window, 1);
}
