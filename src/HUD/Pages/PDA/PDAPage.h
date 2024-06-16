#pragma once

#include "HUD/Pages/Page.h"

class PDAPage : public Page {

public:
    void Init() override;

    void Update() override;

    void DisplayPDAPage();

    void HidePDAPage();

    const char* _backgroundImagePath = "res/Images/PDA/placeholderPDAMenu.png";

};

