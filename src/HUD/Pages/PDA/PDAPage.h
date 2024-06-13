#pragma once

#include "HUD/Pages/Page.h"

class PDAPage : public Page {

public:
    void Init() override;

    void Update() override;

    void DisplayPDAPage();

    void HidePDAPage();

    const char* _PDABackgroundPath = "res/Images/PDA/placeholderPDAMenu.png";

};

