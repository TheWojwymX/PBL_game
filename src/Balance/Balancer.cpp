//
// Created by TheWojwymX on 23.06.2024.
//

#include "Balancer.h"
#include "imgui.h"
#include "../Managers/GameManager.h"
#include "../Enemies/EnemiesManager.h"
#include "../Turrets/TurretsManager.h"


Balancer::Balancer()
{

}

Balancer& Balancer::GetInstance()
{
    static Balancer instance;
    return instance;
}

void Balancer::addToImgui()
{
    ImGui::Begin("Balancer");

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
    ImGui::Text("Damage");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Damage", &_antDamage, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Attack Frequency");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##Attack Frequency", &_antAttackFrequency, 0, 0);
    ImGui::SameLine();
    if (ImGui::Button("Update"))
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
    ImGui::Text("Damage");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##BDamage", &_beetleDamage, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Attack Frequency");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##BAttack Frequency", &_beetleAttackFrequency, 0, 0);
    ImGui::SameLine();
    if (ImGui::Button("Update"))
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
    ImGui::Text("Damage");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##WDamage", &_waspDamage, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Attack Frequency");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##WAttack Frequency", &_waspAttackFrequency, 0, 0);
    ImGui::SameLine();
    if (ImGui::Button("Update"))
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



    /////// Minigun stats ////////
    ImGui::Text("Minigun Stats:");
    ImGui::SetNextItemWidth(50);
    ImGui::Text("Fire Rate");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##FireRate", &_minigunFireRate, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Damage");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##TurretDamage", &_minigunDamage, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Side Range");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##SideRange", &_minigunSideRange, 0, 0);
    ImGui::SameLine();

    ImGui::SetNextItemWidth(50);
    ImGui::Text("Forward Range");
    ImGui::SameLine();
    ImGui::SetNextItemWidth(50);
    ImGui::InputFloat("##ForwardRange", &_minigunForwardRange, 0, 0);
    ImGui::SameLine();
    if (ImGui::Button("Update"))
    {
        TURRETSMANAGER._turretStats[0].x = _minigunFireRate;
        TURRETSMANAGER._turretStats[0].y = _minigunDamage;
        TURRETSMANAGER._turretStats[0].z = _minigunSideRange;
        TURRETSMANAGER._turretStats[0].z = _minigunForwardRange;

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


    ImGui::End();

};
