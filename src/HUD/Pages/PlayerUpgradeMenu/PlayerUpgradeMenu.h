//
// Created by Jacek on 09.05.2024.
//

#ifndef SANDBOX_PLAYERUPGRADEMENU_H
#define SANDBOX_PLAYERUPGRADEMENU_H


#include "HUD/Pages/Page.h"
#include "HUD/Buttons/PlayerUpgrades/UpgradeRadius.h"
#include "HUD/Buttons/PlayerUpgrades/UpgradeReach.h"
#include "HUD/Buttons/PlayerUpgrades/UpgradeSpeed.h"


class PlayerUpgradeMenu : public Page  {
public:

    std::array<float, 32> _backgroundVertices{
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    UpgradeRadius _upgradeRadiusButton;
    UpgradeReach _upgradeReachButton;
    UpgradeSpeed _upgradeSpeedButton;

    void Init() override;

    void Update() override;

    void SetVertices(const array<float, 32> &vertices) override;
};


#endif //SANDBOX_PLAYERUPGRADEMENU_H
