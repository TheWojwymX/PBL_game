#pragma once

#include "HUD/Pages/Page.h"
#include "HUD/Text/TextRenderer.h"
#include "HUD/Buttons/MessagePage/ContinueMessageButton.h"

class MessagePage : public Page  {
public:
    //const char *_generalImagePath = "res/Images/placeholderGeneral.png";
    ImageRenderer _generalImage;

    void Init() override;

    void Update() override;
    string _actualText = "default";

};
