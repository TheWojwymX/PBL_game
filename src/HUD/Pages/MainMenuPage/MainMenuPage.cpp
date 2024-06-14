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
    Page::Init();
}

void MainMenuPage::Update() {
    if (_shouldRender) {
        Page::Update();
        _startButton->Update();
        _settingsButton->Update();
        _creditsButton->Update();
        _quitButton->Update();
    }
}

void MainMenuPage::SetupButtons() {
    _startButton = std::make_shared<Button>("res/Images/Button/start.png",
        "res/Images/Button/start_hover.png",
        "res/Images/Button/start_clicked.png",
        glm::vec2(0, 10),
        [this]() {
            GAMEMANAGER.StartGame();
        });

    _settingsButton = std::make_shared<Button>("res/Images/Button/settings.png",
        "res/Images/Button/settings_hover.png",
        "res/Images/Button/settings_clicked.png",
        glm::vec2(0, 0),
        [this]() {
            SettingsTest();
        });

    _creditsButton = std::make_shared<Button>("res/Images/Button/credits.png",
        "res/Images/Button/credits_hover.png",
        "res/Images/Button/credits_clicked.png",
        glm::vec2(0, -10),
        [this]() {
            CreditsButtonOnClick();
        });

    _quitButton = std::make_shared<Button>("res/Images/Button/quit.png",
        "res/Images/Button/quit_hover.png",
        "res/Images/Button/quit_clicked.png",
        glm::vec2(0, -20),
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
