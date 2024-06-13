#pragma once


#include "HUD/Pages/Page.h"
#include "HUD/Buttons/DomeUpgrades/UpgradeDomeHP.h"

class DomeUpgradeMenu : public Page  {
public:
    UpgradeDomeHP _upgradeDomeHP;

    void Init() override;

    void Update() override;
};
