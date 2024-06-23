//
// Created by TheWojwymX on 23.06.2024.
//

#ifndef SANDBOX_BALANCER_H
#define SANDBOX_BALANCER_H

#define BALANCER Balancer::GetInstance()

class Balancer {
public:
    static Balancer& GetInstance();
    ~Balancer() = default;
    Balancer();
    Balancer(const Balancer&) = delete;
    Balancer& operator=(const Balancer&) = delete;

    void addEnemyStatsToImgui();
    void addTurretStatsToImgui();
    void addPhaseStatsToImgui();
    void addUpgradesStatsToImgui();

    float _antSpeed = 7.0f;
    float _antHp = 3.0f;
    float _antDamage = 0.5f;
    float _antAttackFrequency = 0.6f;

    float _beetleSpeed = 3.0f;
    float _beetleHp = 30.0f;
    float _beetleDamage = 5.0f;
    float _beetleAttackFrequency = 1.5f;

    float _waspSpeed = 10.0f;
    float _waspHp = 2.0f;
    float _waspDamage = 1.0f;
    float _waspAttackFrequency = 0.45f;

    float _minigunFireRate = 0.5f;
    float _minigunDamage = 1.0f;
    float _minigunSideRange = 80.0f;
    float _minigunForwardRange = 60.0f;

    float _sniperFireRate = 2.0f;
    float _sniperDamage = 5.0f;
    float _sniperSideRange = 50.0f;
    float _sniperForwardRange = 100.0f;

    float _rifleFireRate = 1.0f;
    float _rifleDamage = 1.0f;
    float _rifleSideRange = 80.0f;
    float _rifleForwardRange = 80.0f;

    int _antsNumber[10] = {10,25,50,80,115,155,200,250,305,365};
    int _beetlesNumber[10] = {0,1,3,6,10,15,21,28,36,45};
    int _waspsNumber[10] = {0,0,1,3,6,10,15,21,28,36};
    float _endlessNumber[3] = {0.85f, 0.05f, 0.1f};

    float _digPhaseStartTime = 60.0f;
    float _digPercentage = 0.60f;
    float _fightPercentage = 0.40f;
    float _timeIncrement = 10.0f;

    int _hpUpgradeCosts[5] = {5, 10, 15, 20, 25};
    float _hpUpgradeValues[5] = {25.0f, 50.0f, 75.0f, 100.0f, 125.0f};

    int _hpRegenUpgradeCosts[5] = {5, 10, 15, 20, 25};
    float _hpRegenUpgradeValues[5] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};

    int _hpRepairCost = 5;
    float _hpRepairValue = 10.0f;

    int _minigunUpgradeCosts[2] = {5, 15};
    int _sniperUpgradeCosts[2] = {5, 15};
    int _rifleUpgradeCosts[2] = {5, 15};

    float _minigunUpgradeValues[2][4] = { {0.0f, -0.20f, 30, 5}, {0.0f, -0.20f, 30, 5} };
    float _sniperUpgradeValues[2][4] = { {5, 0.0f, 5, 30}, {5, 0.0f, 5, 30} };
    float _rifleUpgradeValues[2][4] = {  {1.0f, -0.25f, 20, 20},  {1.0f, -0.25f, 20, 20} };

    int _jetpackCapacityUpgradeCosts[5] = {5, 10, 15, 20, 25};
    float _jetpackCapacityUpgradeValues[5] = {25.0f, 50.0f, 75.0f, 100.0f, 125.0f};

    int _miningSpeedUpgradeCosts[5] = {5, 10, 15, 20, 25};
    float _miningSpeedUpgradeValues[5] = {0.25f, 0.25f, 0.25f, 0.25f, 0.25f};

    int _miningReachUpgradeCosts[5] = {5, 8, 12, 17, 23};
    float _miningReachUpgradeValues[5] = {2.0f, 3.0f, 4.0f, 5.0f, 6.0f};

    int _miningRadiusUpgradeCosts[5] = {10, 15, 20, 25, 30};
    float _miningRadiusUpgradeValues[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};


};


#endif //SANDBOX_BALANCER_H
