//
// Created by Jacek on 24.06.2024.
//

#include "SettingsPage.h"
#include "Managers/AudioManager.h"
#include "HUD/PageManager.h"

void SettingsPage::Init() {
    SetCorners(glm::vec2(0, 0), glm::vec2(0, 0));
    SetupButtons();
    _musicMinusButton->Init();
    _musicPlusButton->Init();
    _sfxMinusButton->Init();
    _sfxPlusButton->Init();
    _discoButtonOff->Init();
    _controlsButton->Init();
    _backButton->Init();
    _discoButtonOn->Init();
    _buttonsBackground.Init("res/Images/MainMenuSettings/settings_menu.png", CoordsConverter::ConvertCoords(glm::vec2(756, 779)),
                            CoordsConverter::ConvertCoords(glm::vec2(1163, 313)), true, false);
    _logo.Init("res/Images/logo.png", CoordsConverter::ConvertCoords(glm::vec2(696, 224)), CoordsConverter::ConvertCoords(glm::vec2(1235, 118)), true,
               false);
    _backgroundMainMenu.Init(_backgroundMainMenuImagePath, glm::vec2(-50, -50), glm::vec2(50, 50), true, false);

    Page::Init();
}

void SettingsPage::Update() {
    if (_shouldRender) {

        if(_mode == 0){
            _backgroundMainMenu.Render();
        }

        Page::Update();
        _buttonsBackground.Render();
        _musicMinusButton->Update();
        _sfxMinusButton->Update();
        _sfxPlusButton->Update();
        _musicPlusButton->Update();

        _controlsButton->Update();
        _backButton->Update();
        _logo.Render();

        if (_isDisco) {
            _discoButtonOn->Update();
        } else {
            _discoButtonOff->Update();
        }

        glm::vec2 coords = CoordsConverter::ConvertTextCoords(glm::vec2(1070, 369));
        TEXTRENDERER.RenderTextCentered(to_string((int) (AUDIOMANAGER._musicVolume * 100)), coords.x, coords.y, 0.5f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));

        coords = CoordsConverter::ConvertTextCoords(glm::vec2(1070, 415));
        TEXTRENDERER.RenderTextCentered(to_string((int) (AUDIOMANAGER._sfxVolume * 100)), coords.x, coords.y, 0.5f, glm::vec4(0.0f, 0.0f, 0.0f, 1.0f));
    }
}

void SettingsPage::SetupButtons() {
    _musicMinusButton = std::make_shared<Button>("res/Images/Button/settings_minus.png",
                                                 "res/Images/Button/settings_minus_hover.png",
                                                 "res/Images/Button/settings_minus_clicked.png",
                                                 CoordsConverter::ConvertCoords(glm::vec2(1003, 384)),
                                                 CoordsConverter::ConvertCoords(glm::vec2(1033, 354)),
                                                 [this]() {
                                                     AUDIOMANAGER.ChangeMusicVolume(-5);
                                                 });

    _musicPlusButton = std::make_shared<Button>("res/Images/Button/settings_plus.png",
                                                "res/Images/Button/settings_plus_hover.png",
                                                "res/Images/Button/settings_plus_clicked.png",
                                                CoordsConverter::ConvertCoords(glm::vec2(1108, 384)),
                                                CoordsConverter::ConvertCoords(glm::vec2(1138, 354)),
                                                [this]() {
                                                    AUDIOMANAGER.ChangeMusicVolume(5);
                                                });

    _sfxMinusButton = std::make_shared<Button>("res/Images/Button/settings_minus.png",
                                               "res/Images/Button/settings_minus_hover.png",
                                               "res/Images/Button/settings_minus_clicked.png",
                                               CoordsConverter::ConvertCoords(glm::vec2(1003, 430)),
                                               CoordsConverter::ConvertCoords(glm::vec2(1033, 400)),
                                               [this]() {
                                                   AUDIOMANAGER.ChangeSFXVolume(-5);
                                               });

    _sfxPlusButton = std::make_shared<Button>("res/Images/Button/settings_plus.png",
                                              "res/Images/Button/settings_plus_hover.png",
                                              "res/Images/Button/settings_plus_clicked.png",
                                              CoordsConverter::ConvertCoords(glm::vec2(1108, 430)),
                                              CoordsConverter::ConvertCoords(glm::vec2(1138, 400)),
                                              [this]() {
                                                  AUDIOMANAGER.ChangeSFXVolume(5);
                                              });

    _discoButtonOff = std::make_shared<Button>("res/Images/Button/settings_disco.png",
                                               "res/Images/Button/settings_disco_hover.png",
                                               "res/Images/Button/settings_disco_clicked.png",
                                               CoordsConverter::ConvertCoords(glm::vec2(1108, 502)),
                                               CoordsConverter::ConvertCoords(glm::vec2(1138, 472)),
                                               [this]() {
                                                   LIGHTSMANAGER.ChangeDisco();
                                                   _isDisco = !_isDisco;
                                               });

    _discoButtonOn = std::make_shared<Button>("res/Images/Button/settings_disco2.png",
                                              "res/Images/Button/settings_disco2_hover.png",
                                              "res/Images/Button/settings_disco2_clicked.png",
                                              CoordsConverter::ConvertCoords(glm::vec2(1108, 502)),
                                              CoordsConverter::ConvertCoords(glm::vec2(1138, 472)),
                                              [this]() {
                                                  LIGHTSMANAGER.ChangeDisco();
                                                  _isDisco = !_isDisco;
                                              });

    _controlsButton = std::make_shared<Button>("res/Images/Button/button_controls.png",
                                               "res/Images/Button/button_controls_hover.png",
                                               "res/Images/Button/button_controls_clicked.png",
                                               CoordsConverter::ConvertCoords(glm::vec2(781, 622)),
                                               CoordsConverter::ConvertCoords(glm::vec2(1138, 521)),
                                               [this]() {
                                                   if (_mode == 0) {
                                                       PAGEMANAGER.GoToKeyboardSettings(0);
                                                   } else {
                                                       PAGEMANAGER.GoToKeyboardSettings(1);
                                                   }
                                               });

    _backButton = std::make_shared<Button>("res/Images/Button/button_back.png",
                                           "res/Images/Button/button_back_hover.png",
                                           "res/Images/Button/button_back_clicked.png",
                                           CoordsConverter::ConvertCoords(glm::vec2(781, 741)),
                                           CoordsConverter::ConvertCoords(glm::vec2(1138, 640)),
                                           [this]() {
                                               if (_mode == 0) {
                                                   PAGEMANAGER.GoToMainMenu();
                                               } else {
                                                   PAGEMANAGER.GoToPauseMenu();
                                               }
                                           });
}

void SettingsPage::ChangeModeTo(int mode) {
    _mode = mode;
}
