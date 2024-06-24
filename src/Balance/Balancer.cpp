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


    ImGui::Text("Player upgrades:");
    ImGui::SetNextItemWidth(50);
    ImGui::Text("Jetpack Capacity Cost:");

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
    ImGui::Text("Jetpack Capacity value:");

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

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Mining Speed Cost:");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningSpeedCost1", &_miningSpeedUpgradeCosts[0], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningSpeedCost2", &_miningSpeedUpgradeCosts[1], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningSpeedCost3", &_miningSpeedUpgradeCosts[2], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningSpeedCost4", &_miningSpeedUpgradeCosts[3], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningSpeedCost5", &_miningSpeedUpgradeCosts[4], 0, 0);

    ImGui::SameLine();
    if(ImGui::Button("Update##15"))
    {
        for(int i = 0; i < 5; i++)
        {
            UPGRADEMANAGER._miningSpeedUpgrades.upgradeCosts[i] = _miningSpeedUpgradeCosts[i];
        }
    }



    ImGui::SetNextItemWidth(50);
    ImGui::Text("Mining Speed Value:");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningSpeedValue1", &_miningSpeedUpgradeValues[0], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningSpeedValue2", &_miningSpeedUpgradeValues[1], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningSpeedValue3", &_miningSpeedUpgradeValues[2], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningSpeedValue4", &_miningSpeedUpgradeValues[3], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningSpeedValue5", &_miningSpeedUpgradeValues[4], 0, 0);

    ImGui::SameLine();
    if(ImGui::Button("Update##16"))
    {
        for(int i = 0; i < 5; i++)
        {
            UPGRADEMANAGER._miningSpeedUpgrades.upgradeValues[i] = _miningSpeedUpgradeValues[i];
        }
    }


    ImGui::SetNextItemWidth(50);
    ImGui::Text("Mining Reach Cost:");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningReachCost1", &_miningReachUpgradeCosts[0], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningReachCost2", &_miningReachUpgradeCosts[1], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningReachCost3", &_miningReachUpgradeCosts[2], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningReachCost4", &_miningReachUpgradeCosts[3], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningReachCost5", &_miningReachUpgradeCosts[4], 0, 0);

    ImGui::SameLine();
    if(ImGui::Button("Update##17"))
    {
        for(int i = 0; i < 5; i++)
        {
            UPGRADEMANAGER._miningReachUpgrades.upgradeCosts[i] = _miningReachUpgradeCosts[i];
        }
    }



    ImGui::SetNextItemWidth(50);
    ImGui::Text("Mining Reach Value:");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningReachValue1", &_miningReachUpgradeValues[0], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningReachValue2", &_miningReachUpgradeValues[1], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningReachValue3", &_miningReachUpgradeValues[2], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningReachValue4", &_miningReachUpgradeValues[3], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningReachValue5", &_miningReachUpgradeValues[4], 0, 0);

    ImGui::SameLine();
    if(ImGui::Button("Update##18"))
    {
        for(int i = 0; i < 5; i++)
        {
            UPGRADEMANAGER._miningReachUpgrades.upgradeValues[i] = _miningReachUpgradeValues[i];
        }
    }



    ImGui::SetNextItemWidth(50);
    ImGui::Text("Mining Radius Cost:");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningRadiusCost1", &_miningReachUpgradeCosts[0], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningRadiusCost2", &_miningRadiusUpgradeCosts[1], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningRadiusCost3", &_miningRadiusUpgradeCosts[2], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningRadiusCost4", &_miningRadiusUpgradeCosts[3], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##MiningRadiusCost5", &_miningRadiusUpgradeCosts[4], 0, 0);

    ImGui::SameLine();
    if(ImGui::Button("Update##19"))
    {
        for(int i = 0; i < 5; i++)
        {
            UPGRADEMANAGER._miningRadiusUpgrades.upgradeCosts[i] = _miningRadiusUpgradeCosts[i];
        }
    }



    ImGui::SetNextItemWidth(50);
    ImGui::Text("Mining Radius Value:");

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningRadiusValue1", &_miningRadiusUpgradeValues[0], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningRadiusValue2", &_miningRadiusUpgradeValues[1], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningRadiusValue3", &_miningRadiusUpgradeValues[2], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningRadiusValue4", &_miningRadiusUpgradeValues[3], 0, 0);

    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##MiningRadiusValue5", &_miningRadiusUpgradeValues[4], 0, 0);

    ImGui::SameLine();
    if(ImGui::Button("Update##20"))
    {
        for(int i = 0; i < 5; i++)
        {
            UPGRADEMANAGER._miningRadiusUpgrades.upgradeValues[i] = _miningRadiusUpgradeValues[i];
        }
    }



    ImGui::Dummy(ImVec2(0.0f, 10.0f));

    ImGui::Text("Turret Upgrades:");
    ImGui::SetNextItemWidth(50);

    ImGui::Text("Minigun level 2 cost:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Lvl2_minugunCost", &_minigunUpgradeCosts[0], 0, 0);
    ImGui::Text("Dmg:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl2_minigunDamage", &_minigunUpgradeValues[0][0], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Fire Rate:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl2_minigunFireRate", &_minigunUpgradeValues[0][1], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Side:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl2_minigunSide", &_minigunUpgradeValues[0][2], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Forward:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl2_minigunForward", &_minigunUpgradeValues[0][3], 0, 0);

    ImGui::SameLine();
    if (ImGui::Button("Update##21"))
    {
        UPGRADEMANAGER._minigunUpgrades.upgradeCosts[0] = _minigunUpgradeCosts[0];
        UPGRADEMANAGER._minigunUpgrades.upgradeValues[0].x = _minigunUpgradeValues[0][0];
        UPGRADEMANAGER._minigunUpgrades.upgradeValues[0].y = _minigunUpgradeValues[0][1];
        UPGRADEMANAGER._minigunUpgrades.upgradeValues[0].z = _minigunUpgradeValues[0][2];
        UPGRADEMANAGER._minigunUpgrades.upgradeValues[0].w = _minigunUpgradeValues[0][3];
    }

    ImGui::Dummy(ImVec2(0, 5.0f));

    ImGui::Text("Minigun level 3 cost:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Lvl3_minugunCost", &_minigunUpgradeCosts[1], 0, 0);
    ImGui::Text("Dmg:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl3_minigunDamage", &_minigunUpgradeValues[1][0], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Fire Rate:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl3_minigunFireRate", &_minigunUpgradeValues[1][1], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Side:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl3_minigunSide", &_minigunUpgradeValues[1][2], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Forward:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl3_minigunForward", &_minigunUpgradeValues[1][3], 0, 0);

    ImGui::SameLine();
    if (ImGui::Button("Update##22"))
    {
        UPGRADEMANAGER._minigunUpgrades.upgradeCosts[0] = _minigunUpgradeCosts[1];
        UPGRADEMANAGER._minigunUpgrades.upgradeValues[0].x = _minigunUpgradeValues[1][0];
        UPGRADEMANAGER._minigunUpgrades.upgradeValues[0].y = _minigunUpgradeValues[1][1];
        UPGRADEMANAGER._minigunUpgrades.upgradeValues[0].z = _minigunUpgradeValues[1][2];
        UPGRADEMANAGER._minigunUpgrades.upgradeValues[0].w = _minigunUpgradeValues[1][3];
    }

    ImGui::Dummy(ImVec2(0, 5.0f));

    ImGui::Text("Sniper level 2 cost:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Lvl2_SniperCost", &_sniperUpgradeCosts[0], 0, 0);
    ImGui::Text("Dmg:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl2_SniperDamage", &_sniperUpgradeValues[0][0], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Fire Rate:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl2_SniperFireRate", &_sniperUpgradeValues[0][1], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Side:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl2_SniperSide", &_sniperUpgradeValues[0][2], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Forward:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl2_SniperForward", &_sniperUpgradeValues[0][3], 0, 0);

    ImGui::SameLine();
    if (ImGui::Button("Update##23"))
    {
        UPGRADEMANAGER._sniperUpgrades.upgradeCosts[0] = _sniperUpgradeCosts[0];
        UPGRADEMANAGER._sniperUpgrades.upgradeValues[0].x = _sniperUpgradeValues[0][0];
        UPGRADEMANAGER._sniperUpgrades.upgradeValues[0].y = _sniperUpgradeValues[0][1];
        UPGRADEMANAGER._sniperUpgrades.upgradeValues[0].z = _sniperUpgradeValues[0][2];
        UPGRADEMANAGER._sniperUpgrades.upgradeValues[0].w = _sniperUpgradeValues[0][3];
    }

    ImGui::Dummy(ImVec2(0, 5.0f));

    ImGui::Text("Sniper level 3 cost:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Lvl3_SniperCost", &_sniperUpgradeCosts[1], 0, 0);
    ImGui::Text("Dmg:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl3_SniperDamage", &_sniperUpgradeValues[1][0], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Fire Rate:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl3_SniperFireRate", &_sniperUpgradeValues[1][1], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Side:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl3_SniperSide", &_sniperUpgradeValues[1][2], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Forward:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl3_SniperForward", &_sniperUpgradeValues[1][3], 0, 0);

    ImGui::SameLine();
    if (ImGui::Button("Update##24"))
    {
        UPGRADEMANAGER._sniperUpgrades.upgradeCosts[0] = _sniperUpgradeCosts[1];
        UPGRADEMANAGER._sniperUpgrades.upgradeValues[0].x = _sniperUpgradeValues[1][0];
        UPGRADEMANAGER._sniperUpgrades.upgradeValues[0].y = _sniperUpgradeValues[1][1];
        UPGRADEMANAGER._sniperUpgrades.upgradeValues[0].z = _sniperUpgradeValues[1][2];
        UPGRADEMANAGER._sniperUpgrades.upgradeValues[0].w = _sniperUpgradeValues[1][3];
    }


    ImGui::Dummy(ImVec2(0, 5.0f));

    ImGui::Text("Rifle level 2 cost:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Lvl2_RifleCost", &_rifleUpgradeCosts[0], 0, 0);
    ImGui::Text("Dmg:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl2_RifleDamage", &_rifleUpgradeValues[0][0], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Fire Rate:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl2_RifleFireRate", &_rifleUpgradeValues[0][1], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Side:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl2_RifleSide", &_rifleUpgradeValues[0][2], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Forward:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl2_RifleForward", &_rifleUpgradeValues[0][3], 0, 0);

    ImGui::SameLine();
    if (ImGui::Button("Update##25"))
    {
        UPGRADEMANAGER._rifleUpgrades.upgradeCosts[0] = _rifleUpgradeCosts[0];
        UPGRADEMANAGER._rifleUpgrades.upgradeValues[0].x = _rifleUpgradeValues[0][0];
        UPGRADEMANAGER._rifleUpgrades.upgradeValues[0].y = _rifleUpgradeValues[0][1];
        UPGRADEMANAGER._rifleUpgrades.upgradeValues[0].z = _rifleUpgradeValues[0][2];
        UPGRADEMANAGER._rifleUpgrades.upgradeValues[0].w = _rifleUpgradeValues[0][3];
    }

    ImGui::Dummy(ImVec2(0, 5.0f));

    ImGui::Text("Sniper level 3 cost:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputInt("##Lvl3_RifleCost", &_rifleUpgradeCosts[1], 0, 0);
    ImGui::Text("Dmg:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl3_RifleDamage", &_rifleUpgradeValues[1][0], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Fire Rate:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl3_RifleFireRate", &_rifleUpgradeValues[1][1], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Side:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl3_RifleSide", &_rifleUpgradeValues[1][2], 0, 0);

    ImGui::SameLine();
    ImGui::Text("Forward:");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Lvl3_RifleForward", &_rifleUpgradeValues[1][3], 0, 0);

    ImGui::SameLine();
    if (ImGui::Button("Update##26"))
    {
        UPGRADEMANAGER._sniperUpgrades.upgradeCosts[0] = _rifleUpgradeCosts[1];
        UPGRADEMANAGER._sniperUpgrades.upgradeValues[0].x = _rifleUpgradeValues[1][0];
        UPGRADEMANAGER._sniperUpgrades.upgradeValues[0].y = _rifleUpgradeValues[1][1];
        UPGRADEMANAGER._sniperUpgrades.upgradeValues[0].z = _rifleUpgradeValues[1][2];
        UPGRADEMANAGER._sniperUpgrades.upgradeValues[0].w = _rifleUpgradeValues[1][3];
    }

    ImGui::End();
}

void Balancer::resetToDefault()
{
    _antSpeed = 7.0f;
    _antHp = 3.0f;
    _antDamage = 0.5f;
    _antAttackFrequency = 0.6f;

    _beetleSpeed = 3.0f;
    _beetleHp = 30.0f;
    _beetleDamage = 5.0f;
    _beetleAttackFrequency = 1.5f;

    _waspSpeed = 10.0f;
    _waspHp = 2.0f;
    _waspDamage = 1.0f;
    _waspAttackFrequency = 0.45f;

    _minigunFireRate = 0.5f;
    _minigunDamage = 1.0f;
    _minigunSideRange = 80.0f;
    _minigunForwardRange = 60.0f;

    _sniperFireRate = 2.0f;
    _sniperDamage = 5.0f;
    _sniperSideRange = 50.0f;
    _sniperForwardRange = 100.0f;

    _rifleFireRate = 1.0f;
    _rifleDamage = 1.0f;
    _rifleSideRange = 80.0f;
    _rifleForwardRange = 80.0f;

    int _antsNumberCopy[10] = {10,25,50,80,115,155,200,250,305,365};
    std::copy(std::begin(_antsNumberCopy), std::end(_antsNumberCopy), std::begin(_antsNumber));

    int _beetlesNumberCopy[10] = {0,1,3,6,10,15,21,28,36,45};
    std::copy(std::begin(_beetlesNumberCopy), std::end(_beetlesNumberCopy), std::begin(_beetlesNumber));

    int _waspsNumberCopy[10] = {0,0,1,3,6,10,15,21,28,36};
    std::copy(std::begin(_waspsNumberCopy), std::end(_waspsNumberCopy), std::begin(_waspsNumber));

    _endlessNumber[0] = 0.85f;
    _endlessNumber[1] = 0.05f;
    _endlessNumber[2] = 0.1f;

    _hpUpgradeCosts[0] = 5;
    _hpUpgradeCosts[1] = 10;
    _hpUpgradeCosts[2] = 15;
    _hpUpgradeCosts[3] = 20;
    _hpUpgradeCosts[4] = 25;

    _hpUpgradeValues[0] = 25.0f;
    _hpUpgradeValues[1] = 50.0f;
    _hpUpgradeValues[2] = 75.0f;
    _hpUpgradeValues[3] = 100.0f;
    _hpUpgradeValues[4] = 125.0f;

    _hpRegenUpgradeCosts[0] = 5;
    _hpRegenUpgradeCosts[1] = 10;
    _hpRegenUpgradeCosts[2] = 15;
    _hpRegenUpgradeCosts[3] = 20;
    _hpRegenUpgradeCosts[4] = 25;

    _hpRegenUpgradeValues[0] = 0.1f;
    _hpRegenUpgradeValues[1] = 0.2f;
    _hpRegenUpgradeValues[2] = 0.3f;
    _hpRegenUpgradeValues[3] = 0.4f;
    _hpRegenUpgradeValues[4] = 0.5f;

    _hpRepairCost = 5;
    _hpRepairValue = 10.0f;

    _minigunUpgradeCosts[0] = 5;
    _minigunUpgradeCosts[1] = 15;
    _sniperUpgradeCosts[0] = 5;
    _sniperUpgradeCosts[1] = 15;
    _rifleUpgradeCosts[0] = 5;
    _rifleUpgradeCosts[1] = 15;

    _minigunUpgradeValues[0][0] = 0.0f;
    _minigunUpgradeValues[0][1] = -0.20f;
    _minigunUpgradeValues[0][2] = 30.0f;
    _minigunUpgradeValues[0][3] = 5.0f;
    _minigunUpgradeValues[1][0] = 0.0f;
    _minigunUpgradeValues[1][1] = -0.20f;
    _minigunUpgradeValues[1][2] = 30.0f;
    _minigunUpgradeValues[1][3] = 5.0f;

    _sniperUpgradeValues[0][0] = 5.0f;
    _sniperUpgradeValues[0][1] = 0.0f;
    _sniperUpgradeValues[0][2] = 5.0f;
    _sniperUpgradeValues[0][3] = 30.0f;
    _sniperUpgradeValues[1][0] = 5.0f;
    _sniperUpgradeValues[1][1] = 0.0f;
    _sniperUpgradeValues[1][2] = 5.0f;
    _sniperUpgradeValues[1][3] = 30.0f;

    _rifleUpgradeValues[0][0] = 1.0f;
    _rifleUpgradeValues[0][1] = -0.25f;
    _rifleUpgradeValues[0][2] = 20.0f;
    _rifleUpgradeValues[0][3] = 20.0f;
    _rifleUpgradeValues[1][0] = 1.0f;
    _rifleUpgradeValues[1][1] = -0.25f;
    _rifleUpgradeValues[1][2] = 20.0f;
    _rifleUpgradeValues[1][3] = 20.0f;

    _jetpackCapacityUpgradeCosts[0] = 5;
    _jetpackCapacityUpgradeCosts[1] = 10;
    _jetpackCapacityUpgradeCosts[2] = 15;
    _jetpackCapacityUpgradeCosts[3] = 20;
    _jetpackCapacityUpgradeCosts[4] = 25;

    _jetpackCapacityUpgradeValues[0] = 25.0f;
    _jetpackCapacityUpgradeValues[1] = 50.0f;
    _jetpackCapacityUpgradeValues[2] = 75.0f;
    _jetpackCapacityUpgradeValues[3] = 100.0f;
    _jetpackCapacityUpgradeValues[4] = 125.0f;

    _miningSpeedUpgradeCosts[0] = 5;
    _miningSpeedUpgradeCosts[1] = 10;
    _miningSpeedUpgradeCosts[2] = 15;
    _miningSpeedUpgradeCosts[3] = 20;
    _miningSpeedUpgradeCosts[4] = 25;

    _miningSpeedUpgradeValues[0] = 0.25f;
    _miningSpeedUpgradeValues[1] = 0.25f;
    _miningSpeedUpgradeValues[2] = 0.25f;
    _miningSpeedUpgradeValues[3] = 0.25f;
    _miningSpeedUpgradeValues[4] = 0.25f;

    _miningReachUpgradeCosts[0] = 5;
    _miningReachUpgradeCosts[1] = 8;
    _miningReachUpgradeCosts[2] = 12;
    _miningReachUpgradeCosts[3] = 17;
    _miningReachUpgradeCosts[4] = 23;

    _miningReachUpgradeValues[0] = 2.0f;
    _miningReachUpgradeValues[1] = 3.0f;
    _miningReachUpgradeValues[2] = 4.0f;
    _miningReachUpgradeValues[3] = 5.0f;
    _miningReachUpgradeValues[4] = 6.0f;

    _miningRadiusUpgradeCosts[0] = 10;
    _miningRadiusUpgradeCosts[1] = 15;
    _miningRadiusUpgradeCosts[2] = 20;
    _miningRadiusUpgradeCosts[3] = 25;
    _miningRadiusUpgradeCosts[4] = 30;

    _miningRadiusUpgradeValues[0] = 1.0f;
    _miningRadiusUpgradeValues[1] = 1.0f;
    _miningRadiusUpgradeValues[2] = 1.0f;
    _miningRadiusUpgradeValues[3] = 1.0f;
    _miningRadiusUpgradeValues[4] = 1.0f;
}
