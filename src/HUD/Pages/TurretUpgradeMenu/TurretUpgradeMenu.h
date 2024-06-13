#pragma once

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/TurretUpgrades/UpgradeDamage.h"
#include "HUD/Buttons/TurretUpgrades/UpgradeFireRate.h"

class TurretUpgradeMenu : public Page  {
public:
    UpgradeDamage _upgradeDamage;
    UpgradeFireRate _upgradeFireRate;

    void Init() override;

    void Update() override;
};
