#pragma once

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/Button.h"
#include "HUD/CoordsConverter.h"

class PlayerUpgradeMenu : public Page  {
public:
    void Init() override;

    void Update() override;

    const char* _backgroundImagePath = "res/Images/UpgradeMenus/update_background1.png";

    std::shared_ptr<Button> _capacityUpgradeButton;
    std::shared_ptr<Button> _reachUpgradeButton;
    std::shared_ptr<Button> _radiusUpgradeButton;
    std::shared_ptr<Button> _speedUpgradeButton;

private:
    void SetupButtons();
};