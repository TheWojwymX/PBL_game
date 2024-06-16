#pragma once

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/Button.h"
#include "HUD/CoordsConverter.h"

class DomeUpgradeMenu : public Page  {
public:
    void Init() override;

    void Update() override;

    const char* _backgroundImagePath = "res/Images/UpgradeMenus/update_background1.png";

    std::shared_ptr<Button> _maxHPUpgradeButton;
    std::shared_ptr<Button> _HPRegenButton;
    std::shared_ptr<Button> _repairButton;

private:
    void SetupButtons();
};
