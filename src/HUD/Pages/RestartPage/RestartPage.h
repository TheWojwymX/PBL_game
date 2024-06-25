//
// Created by Jacek on 18.06.2024.
//

#ifndef SANDBOX_RESTARTPAGE_H
#define SANDBOX_RESTARTPAGE_H

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/Button.h"
#include "Managers/GameManager.h"

class RestartPage : public Page {
public:
    void Init() override;
    void Update() override;

    const char* _backgroundImagePath = "res/Images/GameOverBackground.png";

    std::shared_ptr<Button> _restartButton;
    std::shared_ptr<Button> _mainMenuButton;
    ImageRenderer _logo;
    ImageRenderer _gameOver;

private:
    void SetupButtons();
};


#endif //SANDBOX_RESTARTPAGE_H
