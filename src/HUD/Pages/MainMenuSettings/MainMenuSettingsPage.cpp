//
// Created by Jacek on 24.06.2024.
//

#include "MainMenuSettingsPage.h"
#include "Managers/AudioManager.h"
#include "HUD/PageManager.h"

void MainMenuSettingsPage::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetCorners(glm::vec2(-50, -50), glm::vec2(50, 50));
    SetupButtons();
    _musicMinusButton->Init();
    _musicPlusButton->Init();
    _sfxMinusButton->Init();
    _sfxPlusButton->Init();
    _discoButton->Init();
    _controlsButton->Init();
    _backButton->Init();
    _buttonsBackground.Init("res/Images/MainMenuSettings/settings_menu.png", CoordsConverter::ConvertCoords(glm::vec2(756, 836)), CoordsConverter::ConvertCoords(glm::vec2(1163, 370)), true, false);
    _logo.Init("res/Images/logo.png", CoordsConverter::ConvertCoords(glm::vec2(696, 224)), CoordsConverter::ConvertCoords(glm::vec2(1235, 118)), true, false);
    Page::Init();
}

void MainMenuSettingsPage::Update() {
    if (_shouldRender) {
        Page::Update();
        _buttonsBackground.Render();
        _musicMinusButton->Update();
        _sfxMinusButton->Update();
        _sfxPlusButton->Update();
        _musicPlusButton->Update();
        _discoButton->Update();
        _controlsButton->Update();
        _backButton->Update();
        _logo.Render();

        glm::vec2 coords = CoordsConverter::ConvertTextCoords(glm::vec2(1070, 426));
        TEXTRENDERER.RenderTextCentered(to_string((int)(AUDIOMANAGER._musicVolume * 100)), coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));

        coords = CoordsConverter::ConvertTextCoords(glm::vec2(1070, 472));
        TEXTRENDERER.RenderTextCentered(to_string((int)(AUDIOMANAGER._sfxVolume * 100)), coords.x, coords.y, 0.5f, glm::vec3(0.0f, 0.0f, 0.0f));
    }
}

void MainMenuSettingsPage::SetupButtons() {
    _musicMinusButton = std::make_shared<Button>("res/Images/Button/settings_minus.png",
                                            "res/Images/Button/settings_minus_hover.png",
                                            "res/Images/Button/settings_minus_clicked.png",
                                            CoordsConverter::ConvertCoords(glm::vec2(1003, 441)),
                                            CoordsConverter::ConvertCoords(glm::vec2(1033, 411)),
                                            [this]() {
                                                AUDIOMANAGER.ChangeMusicVolume(-5);
                                            });

    _musicPlusButton = std::make_shared<Button>("res/Images/Button/settings_plus.png",
                                               "res/Images/Button/settings_plus_hover.png",
                                               "res/Images/Button/settings_plus_clicked.png",
                                               CoordsConverter::ConvertCoords(glm::vec2(1108, 441)),
                                               CoordsConverter::ConvertCoords(glm::vec2(1138, 411)),
                                               [this]() {
                                                   AUDIOMANAGER.ChangeMusicVolume(5);
                                               });

    _sfxMinusButton = std::make_shared<Button>("res/Images/Button/settings_minus.png",
                                                 "res/Images/Button/settings_minus_hover.png",
                                                 "res/Images/Button/settings_minus_clicked.png",
                                                 CoordsConverter::ConvertCoords(glm::vec2(1003, 487)),
                                                 CoordsConverter::ConvertCoords(glm::vec2(1033, 457)),
                                                 [this]() {
                                                     AUDIOMANAGER.ChangeSFXVolume(-5);
                                                 });

    _sfxPlusButton = std::make_shared<Button>("res/Images/Button/settings_plus.png",
                                                "res/Images/Button/settings_plus_hover.png",
                                                "res/Images/Button/settings_plus_clicked.png",
                                                CoordsConverter::ConvertCoords(glm::vec2(1108, 487)),
                                                CoordsConverter::ConvertCoords(glm::vec2(1138, 457)),
                                                [this]() {
                                                    AUDIOMANAGER.ChangeSFXVolume(5);
                                                });

    _discoButton = std::make_shared<Button>("res/Images/Button/settings_disco.png",
                                              "res/Images/Button/settings_disco_hover.png",
                                              "res/Images/Button/settings_disco_clicked.png",
                                              CoordsConverter::ConvertCoords(glm::vec2(1108, 559)),
                                              CoordsConverter::ConvertCoords(glm::vec2(1138, 529)),
                                              [this]() {

                                              });

    _controlsButton = std::make_shared<Button>("res/Images/Button/button_controls.png",
                                           "res/Images/Button/button_controls_hover.png",
                                           "res/Images/Button/button_controls_clicked.png",
                                           CoordsConverter::ConvertCoords(glm::vec2(781, 679)),
                                           CoordsConverter::ConvertCoords(glm::vec2(1138, 578)),
                                           [this]() {
                                                PAGEMANAGER.GoToKeyboardSettings();
                                           });

    _backButton = std::make_shared<Button>("res/Images/Button/button_back.png",
                                           "res/Images/Button/button_back_hover.png",
                                           "res/Images/Button/button_back_clicked.png",
                                           CoordsConverter::ConvertCoords(glm::vec2(781, 798)),
                                           CoordsConverter::ConvertCoords(glm::vec2(1138, 697)),
                                           [this]() {
                                                PAGEMANAGER.GoToMainMenu();
                                           });
}