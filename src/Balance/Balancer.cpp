//
// Created by TheWojwymX on 23.06.2024.
//

#include "Balancer.h"
#include "imgui.h"
#include "../Managers/GameManager.h"
#include "../Enemies/EnemiesManager.h"
#include "../Turrets/TurretsManager.h"
#include "../Managers/UpgradeManager.h"


Balancer::Balancer()
{

}

Balancer& Balancer::GetInstance()
{
    static Balancer instance;
    return instance;
}

void Balancer::addEnemyStatsToImgui() {
    ImGui::Begin("Enemy Stats");

    /////// Ant ////////
    ImGui::Text("Ant Stats:");
    ImGui::SetNextItemWidth(50);
    ImGui::Text("Speed");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Speed", &_antSpeed, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("HP");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Hp", &_antHp, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Dmg");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Damage", &_antDamage, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Att.Freq.");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Attack Frequency", &_antAttackFrequency, 0, 0);
    ImGui::SameLine();
    if (ImGui::Button("Update##1"))
    {
        ENEMIESMANAGER._enemyStats[0].speed = _antSpeed;
        ENEMIESMANAGER._enemyStats[0].hp = _antHp;
        ENEMIESMANAGER._enemyStats[0].damage = _antDamage;
        ENEMIESMANAGER._enemyStats[0].attackFrequency = _antAttackFrequency;

        for(int i = 0; i < ENEMIESMANAGER._enemies.size(); i++)
        {
            if(ENEMIESMANAGER._enemies[i]->_enemyType == ANT)
            {
                ENEMIESMANAGER._enemies[i]->_speed = _antSpeed;
                ENEMIESMANAGER._enemies[i]->_hp = _antHp;
                ENEMIESMANAGER._enemies[i]->_damage = _antDamage;
                ENEMIESMANAGER._enemies[i]->_attackFrequency = _antAttackFrequency;
            }
        }
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));


    /////// Beetle ////////
    ImGui::Text("Beetle Stats:");
    ImGui::SetNextItemWidth(50);
    ImGui::Text("Speed");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##BSpeed", &_beetleSpeed, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("HP");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##BHp", &_beetleHp, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Dmg");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##BDamage", &_beetleDamage, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Att.Freq.");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##BAttack Frequency", &_beetleAttackFrequency, 0, 0);
    ImGui::SameLine();
    if (ImGui::Button("Update##2"))
    {
        ENEMIESMANAGER._enemyStats[1].speed = _beetleSpeed;
        ENEMIESMANAGER._enemyStats[1].hp = _beetleHp;
        ENEMIESMANAGER._enemyStats[1].damage = _beetleDamage;
        ENEMIESMANAGER._enemyStats[1].attackFrequency = _beetleAttackFrequency;

        for(int i = 0; i < ENEMIESMANAGER._enemies.size(); i++)
        {
            if(ENEMIESMANAGER._enemies[i]->_enemyType == BEETLE)
            {
                ENEMIESMANAGER._enemies[i]->_speed = _beetleSpeed;
                ENEMIESMANAGER._enemies[i]->_hp = _beetleHp;
                ENEMIESMANAGER._enemies[i]->_damage = _beetleDamage;
                ENEMIESMANAGER._enemies[i]->_attackFrequency = _beetleAttackFrequency;
            }
        }
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));



    /////// WASP ////////
    ImGui::Text("Wasp Stats:");
    ImGui::SetNextItemWidth(50);
    ImGui::Text("Speed");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##WSpeed", &_waspSpeed, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("HP");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##WHp", &_waspHp, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Dmg");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##WDamage", &_waspDamage, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Att.Freq.");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##WAttack Frequency", &_waspAttackFrequency, 0, 0);
    ImGui::SameLine();
    if (ImGui::Button("Update##3"))
    {
        ENEMIESMANAGER._enemyStats[2].speed = _waspSpeed;
        ENEMIESMANAGER._enemyStats[2].hp = _waspHp;
        ENEMIESMANAGER._enemyStats[2].damage = _waspDamage;
        ENEMIESMANAGER._enemyStats[2].attackFrequency = _waspAttackFrequency;

        for(int i = 0; i < ENEMIESMANAGER._enemies.size(); i++)
        {
            if(ENEMIESMANAGER._enemies[i]->_enemyType == WASP)
            {
                ENEMIESMANAGER._enemies[i]->_speed = _waspSpeed;
                ENEMIESMANAGER._enemies[i]->_hp = _waspHp;
                ENEMIESMANAGER._enemies[i]->_damage = _waspDamage;
                ENEMIESMANAGER._enemies[i]->_attackFrequency = _waspAttackFrequency;
            }
        }
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::End();
};

void Balancer::addTurretStatsToImgui()
{
    ImGui::Begin("Turret Stats");

    /////// Minigun stats ////////
    ImGui::Text("Minigun Stats:");
    ImGui::SetNextItemWidth(50);
    ImGui::Text("Fire Rate");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##FireRate", &_minigunFireRate, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Dmg");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##TurretDamage", &_minigunDamage, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Side");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##SideRange", &_minigunSideRange, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Forward");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##ForwardRange", &_minigunForwardRange, 0, 0);
    ImGui::SameLine();
    if (ImGui::Button("Update##4"))
    {
        TURRETSMANAGER._turretStats[0].x = _minigunFireRate;
        TURRETSMANAGER._turretStats[0].y = _minigunDamage;
        TURRETSMANAGER._turretStats[0].z = _minigunSideRange;
        TURRETSMANAGER._turretStats[0].w = _minigunForwardRange;

        for(int i = 0; i < TURRETSMANAGER._turrets.size(); i++)
        {
            if (TURRETSMANAGER._turrets[i]->_turretType == MINIGUN)
            {
                TURRETSMANAGER._turrets[i]->_fireRate = _minigunFireRate;
                TURRETSMANAGER._turrets[i]->_damage = _minigunDamage;
                TURRETSMANAGER._turrets[i]->_sideRange = _minigunSideRange;
                TURRETSMANAGER._turrets[i]->_forwardRange = _minigunForwardRange;
            }
        }
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));



    /////// Sniper stats ////////
    ImGui::Text("Sniper Stats:");
    ImGui::SetNextItemWidth(50);
    ImGui::Text("Fire Rate");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##SFireRate", &_sniperFireRate, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Dmg");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##STurretDamage", &_sniperDamage, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Side");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##SSideRange", &_sniperSideRange, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Forward");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##SForwardRange", &_sniperForwardRange, 0, 0);
    ImGui::SameLine();
    if (ImGui::Button("Update##5"))
    {
        TURRETSMANAGER._turretStats[1].x = _sniperFireRate;
        TURRETSMANAGER._turretStats[1].y = _sniperDamage;
        TURRETSMANAGER._turretStats[1].z = _sniperSideRange;
        TURRETSMANAGER._turretStats[1].w = _sniperForwardRange;

        for(int i = 0; i < TURRETSMANAGER._turrets.size(); i++)
        {
            if (TURRETSMANAGER._turrets[i]->_turretType == SNIPER)
            {
                TURRETSMANAGER._turrets[i]->_fireRate = _sniperFireRate;
                TURRETSMANAGER._turrets[i]->_damage = _sniperDamage;
                TURRETSMANAGER._turrets[i]->_sideRange = _sniperSideRange;
                TURRETSMANAGER._turrets[i]->_forwardRange = _sniperForwardRange;
            }
        }
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    /////// Rifle stats ////////
    ImGui::Text("Rifle Stats:");
    ImGui::SetNextItemWidth(50);
    ImGui::Text("Fire Rate");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##RFireRate", &_rifleFireRate, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Dmg");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##RTurretDamage", &_rifleDamage, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Side");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##RSideRange", &_rifleSideRange, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Forward");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##RForwardRange", &_rifleForwardRange, 0, 0);
    ImGui::SameLine();
    if (ImGui::Button("Update##6"))
    {
        TURRETSMANAGER._turretStats[2].x = _rifleFireRate;
        TURRETSMANAGER._turretStats[2].y = _rifleDamage;
        TURRETSMANAGER._turretStats[2].z = _rifleSideRange;
        TURRETSMANAGER._turretStats[2].w = _rifleForwardRange;

        for(int i = 0; i < TURRETSMANAGER._turrets.size(); i++)
        {
            if (TURRETSMANAGER._turrets[i]->_turretType == RIFLE)
            {
                TURRETSMANAGER._turrets[i]->_fireRate = _rifleFireRate;
                TURRETSMANAGER._turrets[i]->_damage = _rifleDamage;
                TURRETSMANAGER._turrets[i]->_sideRange = _rifleSideRange;
                TURRETSMANAGER._turrets[i]->_forwardRange = _rifleForwardRange;
            }
        }
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::End();
}

void Balancer::addPhaseStatsToImgui()
{
    ImGui::Begin("Phases Stats");

//    //////// Czasy fal ////////
//    ImGui::Text("Timers:");
//    ImGui::SetNextItemWidth(50);
//    ImGui::Text("Dig Phase Start Time:");
//    ImGui::SameLine();
//    ImGui::SetNextItemWidth(50);
//    ImGui::InputFloat("##digStartTime", &_digPhaseStartTime, 0, 0);
//
//    ImGui::Text("Dig Percentage:");
//    ImGui::SameLine();
//    ImGui::SetNextItemWidth(50);
//    ImGui::InputFloat("##digPercentage", &_digPercentage, 0, 0);
//
//    ImGui::Text("Fight Percentage:");
//    ImGui::SameLine();
//    ImGui::SetNextItemWidth(50);
//    ImGui::InputFloat("##fightPercentage", &_fightPercentage, 0, 0);
//
//    ImGui::Text("Time increment:");
//    ImGui::SameLine();
//    ImGui::SetNextItemWidth(50);
//    ImGui::InputFloat("##timeIncrement", &_timeIncrement, 0, 0);
//
//    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    //////// Kompozycje fal ////////
    ImGui::Text("Waves (Ant/Beetle/Wasp):");

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Wave 1:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Ants0", &_antsNumber[0], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Beetles0", &_beetlesNumber[0], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Wasps0", &_waspsNumber[0], 0, 0);

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Wave 2:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Ants1", &_antsNumber[1], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Beetles1", &_beetlesNumber[1], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Wasps1", &_waspsNumber[1], 0, 0);

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Wave 3:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Ants2", &_antsNumber[2], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Beetles2", &_beetlesNumber[2], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Wasps2", &_waspsNumber[2], 0, 0);

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Wave 4:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Ants3", &_antsNumber[3], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Beetles3", &_beetlesNumber[3], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Wasps3", &_waspsNumber[3], 0, 0);

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Wave 5:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Ants4", &_antsNumber[4], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Beetles4", &_beetlesNumber[4], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Wasps4", &_waspsNumber[4], 0, 0);

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Wave 6:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Ants5", &_antsNumber[5], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Beetles5", &_beetlesNumber[5], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Wasps5", &_waspsNumber[5], 0, 0);

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Wave 7:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Ants6", &_antsNumber[6], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Beetles6", &_beetlesNumber[6], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Wasps6", &_waspsNumber[6], 0, 0);

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Wave 8:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Ants7", &_antsNumber[7], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Beetles7", &_beetlesNumber[7], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Wasps7", &_waspsNumber[7], 0, 0);

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Wave 9:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Ants8", &_antsNumber[8], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Beetles8", &_beetlesNumber[8], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Wasps8", &_waspsNumber[8], 0, 0);

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Wave 10:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Ants9", &_antsNumber[9], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Beetles9", &_beetlesNumber[9], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Wasps9", &_waspsNumber[9], 0, 0);

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Endless:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##AntsE", &_endlessNumber[0], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##BeetlesE", &_endlessNumber[1], 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##WaspsE", &_endlessNumber[2], 0, 0);

    if (ImGui::Button("Update##7"))
    {
        for(int i = 0; i < ENEMIESMANAGER._roundsInfo.size() - 1; i++)
        {
            ENEMIESMANAGER._roundsInfo[i].x = _antsNumber[i];
            ENEMIESMANAGER._roundsInfo[i].y = _beetlesNumber[i];
            ENEMIESMANAGER._roundsInfo[i].z = _waspsNumber[i];
        }

        ENEMIESMANAGER._roundsInfo[10].x = _endlessNumber[0];
        ENEMIESMANAGER._roundsInfo[10].y = _endlessNumber[1];
        ENEMIESMANAGER._roundsInfo[10].z = _endlessNumber[2];
    }


    ImGui::End();
}

void Balancer::addUpgradesStatsToImgui()
{
    ImGui::Begin("Upgrade costs and values");

    ImGui::Text("Dome Upgrades:");
    ImGui::SetNextItemWidth(50);
    ImGui::Text("Hp upgrade costs:");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##DomeHpCost1", &_hpUpgradeCosts[0], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##DomeHpCost2", &_hpUpgradeCosts[1], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##DomeHpCost3", &_hpUpgradeCosts[2], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##DomeHpCost4", &_hpUpgradeCosts[3], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##DomeHpCost5", &_hpUpgradeCosts[4], 0, 0);

    ImGui::SameLine();
    if(ImGui::Button("Update##8"))
    {
        for(int i = 0; i < 5; i++)
        {
            UPGRADEMANAGER._domeHPUpgrades.upgradeCosts[i] = _hpUpgradeCosts[i];
        }
    }

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Hp upgrade values:");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##DomeHpValues1", &_hpUpgradeValues[0], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##DomeHpValues2", &_hpUpgradeValues[1], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##DomeHpValues3", &_hpUpgradeValues[2], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##DomeHpValues4", &_hpUpgradeValues[3], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##DomeHpValues5", &_hpUpgradeValues[4], 0, 0);

    ImGui::SameLine();
    if(ImGui::Button("Update##9"))
    {
        for(int i = 0; i < 5; i++)
        {
            UPGRADEMANAGER._domeHPUpgrades.upgradeValues[i] = _hpUpgradeValues[i];
        }
    }

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Hp regen costs:");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##DomeHpRegenCost1", &_hpRegenUpgradeCosts[0], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##DomeHpRegenCost2", &_hpRegenUpgradeCosts[1], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##DomeHpRegenCost3", &_hpRegenUpgradeCosts[2], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##DomeHpRegenCost4", &_hpRegenUpgradeCosts[3], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##DomeHpRegenCost5", &_hpRegenUpgradeCosts[4], 0, 0);

    ImGui::SameLine();
    if(ImGui::Button("Update##10"))
    {
        for(int i = 0; i < 5; i++)
        {
            UPGRADEMANAGER._domeHPRegenUpgrades.upgradeCosts[i] = _hpRegenUpgradeCosts[i];
        }
    }

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Hp regen values:");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##DomeHpRegenValues1", &_hpRegenUpgradeValues[0], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##DomeHpRegenValues2", &_hpRegenUpgradeValues[1], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##DomeHpRegenValues3", &_hpRegenUpgradeValues[2], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##DomeHpRegenValues4", &_hpRegenUpgradeValues[3], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##DomeHpRegenValues5", &_hpRegenUpgradeValues[4], 0, 0);

    ImGui::SameLine();
    if(ImGui::Button("Update##11"))
    {
        for(int i = 0; i < 5; i++)
        {
            UPGRADEMANAGER._domeHPRegenUpgrades.upgradeValues[i] = _hpRegenUpgradeValues[i];
        }
    }

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Repair cost:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##HpRepairCost1", &_hpRepairCost, 0, 0);
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::Text("Repair value:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##HpRepairValue1", &_hpRepairValue, 0, 0);
    ImGui::SameLine();
    if(ImGui::Button("Update##12"))
    {
        UPGRADEMANAGER._domeHPRepair.upgradeValues[0] = _hpRepairValue;
        UPGRADEMANAGER._domeHPRepair.upgradeCosts[0] = _hpRepairCost;
    }
    ImGui::Dummy(ImVec2(0.0f, 10.0f));


    ImGui::Text("Jetpack upgrades:");
    ImGui::SetNextItemWidth(50);
    ImGui::Text("Capacity Cost:");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##JetpackCapacityCost1", &_jetpackCapacityUpgradeCosts[0], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##JetpackCapacityCost2", &_jetpackCapacityUpgradeCosts[1], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##JetpackCapacityCost3", &_jetpackCapacityUpgradeCosts[2], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##JetpackCapacityCost4", &_jetpackCapacityUpgradeCosts[3], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##JetpackCapacityCost5", &_jetpackCapacityUpgradeCosts[4], 0, 0);

    ImGui::SameLine();
    if(ImGui::Button("Update##13"))
    {
        for(int i = 0; i < 5; i++)
        {
            UPGRADEMANAGER._jetpackCapacityUpgrades.upgradeCosts[i] = _jetpackCapacityUpgradeCosts[i];
        }
    }



    ImGui::SetNextItemWidth(50);
    ImGui::Text("Capacity value:");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##JetpackCapacityValue1", &_jetpackCapacityUpgradeValues[0], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##JetpackCapacityValue2", &_jetpackCapacityUpgradeValues[1], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##JetpackCapacityValue3", &_jetpackCapacityUpgradeValues[2], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##JetpackCapacityValue4", &_jetpackCapacityUpgradeValues[3], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##JetpackCapacityValue5", &_jetpackCapacityUpgradeValues[4], 0, 0);

    ImGui::SameLine();
    if(ImGui::Button("Update##14"))
    {
        for(int i = 0; i < 5; i++)
        {
            UPGRADEMANAGER._jetpackCapacityUpgrades.upgradeValues[i] = _jetpackCapacityUpgradeValues[i];
        }
    }





    ImGui::End();

//    Upgrades _miningSpeedUpgrades;
//    Upgrades _miningReachUpgrades;
//    Upgrades _miningRadiusUpgrades;

    //    TurretUpgrades _minigunUpgrades;
//    TurretUpgrades _sniperUpgrades;
//    TurretUpgrades _rifleUpgrades;




//    int _hpUpgradeCosts[5] = {5, 10, 15, 20, 25};
//    float _hpUpgradeValues[5] = {25.0f, 50.0f, 75.0f, 100.0f, 125.0f};
//
//    int _hpRegenUpgradeCosts[5] = {5, 10, 15, 20, 25};
//    float _hpRegenUpgradeValues[5] = {0.1f, 0.2f, 0.3f, 0.4f, 0.5f};
//
//    int _hpRepairCost = 5;
//    float _hpRepairValue = 10.0f;
//
//    int _minigunUpgradeCosts[2] = {5, 15};
//    int _sniperUpgradeCosts[2] = {5, 15};
//    int _rifleUpgradeCosts[2] = {5, 15};
//
//    float _minigunUpgradeValues[2][4] = { {0.0f, -0.20f, 30, 5}, {0.0f, -0.20f, 30, 5} };
//    float _sniperUpgradeValues[2][4] = { {5, 0.0f, 5, 30}, {5, 0.0f, 5, 30} };
//    float _rifleUpgradeValues[2][4] = {  {1.0f, -0.25f, 20, 20},  {1.0f, -0.25f, 20, 20} };
//
//    int _jetpackCapacityUpgradeCosts[5] = {5, 10, 15, 20, 25};
//    float _jetpackCapacityUpgradeValues[5] = {25.0f, 50.0f, 75.0f, 100.0f, 125.0f};
//
//    int _miningSpeedUpgradeCosts[5] = {5, 10, 15, 20, 25};
//    float _miningSpeedUpgradeValues[5] = {0.25f, 0.25f, 0.25f, 0.25f, 0.25f};
//
//    int _miningReachUpgradeCosts[5] = {5, 8, 12, 17, 23};
//    float _miningReachUpgradeValues[5] = {2.0f, 3.0f, 4.0f, 5.0f, 6.0f};
//
//    int _miningRadiusUpgradeCosts[5] = {10, 15, 20, 25, 30};
//    float _miningRadiusUpgradeValues[5] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f};


}
