//
// Created by Jacek on 09.05.2024.
//

#include "PauseMenuPage.h"

void PauseMenuPage::Init() {
    _resumeButton.Init();
    Page::Init();
}

void PauseMenuPage::Update() {
    _resumeButton.Update();
    Page::Update();
}
