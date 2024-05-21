//
// Created by Jacek on 21.05.2024.
//

#ifndef SANDBOX_DOMEUPGRADEMENU_H
#define SANDBOX_DOMEUPGRADEMENU_H


#include "HUD/Pages/Page.h"
#include "HUD/Buttons/DomeUpgrades/UpgradeDomeHP.h"

class DomeUpgradeMenu : public Page  {
public:

    std::array<float, 32> _backgroundVertices{
            // positions          // colors           // texture coords
            0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
            0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
            -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
            -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left
    };

    UpgradeDomeHP _upgradeDomeHP;

    void Init() override;

    void Update() override;

    void SetVertices(const array<float, 32> &vertices) override;
};


#endif //SANDBOX_DOMEUPGRADEMENU_H
