//
// Created by Jacek on 26.06.2024.
//

#include "CreditsPage.h"
#include "HUD/PageManager.h"

void CreditsPage::Init() {
    SetBackgroundImagePath(_backgroundImagePath);
    SetCorners(glm::vec2(-50, -50), glm::vec2(50, 50));
    SetupButtons();

    _upButton->Init();
    _downButton->Init();
    _upButtonBlocked->Init();
    _downButtonBlocked->Init();

    for (int i = 0; i < 5; i++) {
        std::string path = "res/Images/Credits/credits_" + std::to_string(i + 1) + "_1.png";
        const char *c_path = path.c_str();
        shared_ptr<ImageRenderer> credit = make_shared<ImageRenderer>();
        credit->Init(c_path, CoordsConverter::ConvertCoords(glm::vec2(169, 1080)), CoordsConverter::ConvertCoords(glm::vec2(1750, 140)), true, false);
        _credits.push_back(credit);
    }

    _backButton->Init();

    Page::Init();
}

void CreditsPage::Update() {
    if (_shouldRender) {
        Page::Update();

        _credits[_pageIndex]->Render();

        if(_pageIndex == 0){
            _downButton->Update();
            _upButtonBlocked->Update();
        } else if(_pageIndex == 4){
            _upButton->Update();
            _downButtonBlocked->Update();
        }else{
            _upButton->Update();
            _downButton->Update();
        }

        _backButton->Update();
    }
}

void CreditsPage::SetupButtons() {
    _upButton = std::make_shared<Button>("res/Images/Button/credits_up.png",
                                         "res/Images/Button/credits_up_hover.png",
                                         "res/Images/Button/credits_up_clicked.png",
                                         CoordsConverter::ConvertCoords(glm::vec2(1618, 530)),
                                         CoordsConverter::ConvertCoords(glm::vec2(1692, 456)),
                                         [this]() {
                                                if(_pageIndex > 0){
                                                    _pageIndex--;
                                                }
                                         });

    _downButton = std::make_shared<Button>("res/Images/Button/credits_down.png",
                                           "res/Images/Button/credits_down_hover.png",
                                           "res/Images/Button/credits_down_clicked.png",
                                           CoordsConverter::ConvertCoords(glm::vec2(1618, 623)),
                                           CoordsConverter::ConvertCoords(glm::vec2(1692, 549)),
                                           [this]() {
                                               if(_pageIndex < 4){
                                                   _pageIndex++;
                                               }
                                           });

    _upButtonBlocked = std::make_shared<Button>("res/Images/Button/credits_up_block.png",
                                                "res/Images/Button/credits_up_block.png",
                                                "res/Images/Button/credits_up_block.png",
                                                CoordsConverter::ConvertCoords(glm::vec2(1618, 530)),
                                                CoordsConverter::ConvertCoords(glm::vec2(1692, 456)),
                                                [this]() {

                                                });

    _downButtonBlocked = std::make_shared<Button>("res/Images/Button/credits_down_block.png",
                                                  "res/Images/Button/credits_down_block.png",
                                                  "res/Images/Button/credits_down_block.png",
                                                  CoordsConverter::ConvertCoords(glm::vec2(1618, 623)),
                                                  CoordsConverter::ConvertCoords(glm::vec2(1692, 549)),
                                                  [this]() {

                                                  });

    _backButton = std::make_shared<Button>("res/Images/Button/button_back.png",
                                           "res/Images/Button/button_back_hover.png",
                                           "res/Images/Button/button_back_clicked.png",
                                           CoordsConverter::ConvertCoords(glm::vec2(781, 1057)),
                                           CoordsConverter::ConvertCoords(glm::vec2(1138, 956)),
                                           [this]() {
                                               PAGEMANAGER.GoToMainMenu();
                                           });
}