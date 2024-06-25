#include "PauseMenuPage.h"
#include "Managers/GameManager.h"
#include "HUD/PageManager.h"

void PauseMenuPage::Init() {
    SetCorners(glm::vec2(0, 0), glm::vec2(0, 0));
    SetupButtons();
    _logo.Init("res/Images/logo.png", CoordsConverter::ConvertCoords(glm::vec2(696, 224)), CoordsConverter::ConvertCoords(glm::vec2(1235, 118)), true, false);
    _resumeButton->Init();
    _settingsButton->Init();
    _quitButton->Init();
    Page::Init();
}

void PauseMenuPage::Update() {
    if (_shouldRender) {
        Page::Update();
        //_backgroundImage.Render();
        _logo.Render();
        _resumeButton->Update();
        _settingsButton->Update();
        _quitButton->Update();
    }
}

void PauseMenuPage::SetupButtons() {
    _resumeButton = std::make_shared<Button>("res/Images/Button/resume.png",
                                             "res/Images/Button/resume_hover.png",
                                             "res/Images/Button/resume_clicked.png",
                                             CoordsConverter::ConvertCoords(glm::vec2(782, 469)),
                                             CoordsConverter::ConvertCoords(glm::vec2(1137, 370)),
                                             [this]() {
                                                 GAMEMANAGER.Unpause();
                                             });

    _settingsButton = std::make_shared<Button>("res/Images/Button/settings.png",
                                               "res/Images/Button/settings_hover.png",
                                               "res/Images/Button/settings_clicked.png",
                                               CoordsConverter::ConvertCoords(glm::vec2(782, 589)),
                                               CoordsConverter::ConvertCoords(glm::vec2(1137, 490)),
                                               [this]() {
                                                   PAGEMANAGER.GoToSettings(1);
                                               });

    _quitButton = std::make_shared<Button>("res/Images/Button/quit.png",
                                           "res/Images/Button/quit_hover.png",
                                           "res/Images/Button/quit_clicked.png",
                                           CoordsConverter::ConvertCoords(glm::vec2(782, 709)),
                                           CoordsConverter::ConvertCoords(glm::vec2(1137, 610)),
                                           [this]() {
                                               glfwSetWindowShouldClose(GAMEMANAGER._window, 1);
                                           });
}