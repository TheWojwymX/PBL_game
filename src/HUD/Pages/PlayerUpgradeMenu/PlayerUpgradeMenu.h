#pragma once

#include "HUD/Pages/Page.h"
#include "HUD/Buttons/Player/UpgradeRadius.h"
#include "HUD/Buttons/Player/UpgradeReach.h"
#include "HUD/Buttons/Player/UpgradeSpeed.h"
#include "HUD/Buttons/Player/JetpackRefuel.h"

class PlayerUpgradeMenu : public Page  {
public:
    UpgradeRadius _upgradeRadiusButton;
    UpgradeReach _upgradeReachButton;
    UpgradeSpeed _upgradeSpeedButton;
    JetpackRefuel _jetpackRefuelButton;

    void Init() override;

    void Update() override;
};