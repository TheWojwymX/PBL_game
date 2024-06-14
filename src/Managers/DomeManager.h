#pragma once

#include <glm/glm.hpp>
#include "Core/Time.h"
#include <iostream>

#define DOMEMANAGER DomeManager::getInstance()

class DomeManager {
public:
    static DomeManager& getInstance();
    DomeManager(const DomeManager&) = delete;
    DomeManager& operator=(const DomeManager&) = delete;

    void TakeDamage(float value);
    void Update();
    void RegenHP();

    float GetDomeHP() { return _hp; }
    float GetDomeMaxHP() { return _maxHP; }
    char GetDomeHPLevel() { return _domeHPLevel; }
    char GetDomeHPRegenLevel() { return _domeHPRegenLevel; }
    void UpgradeDomeHP(float value) { _maxHP += value; _hp += static_cast<int>(value); _domeHPLevel++; }
    void UpgradeDomeHPRegen(float value) { _regenRate += value; _domeHPRegenLevel++; }
    void RepairDomeHP(float value);

private:
    char _domeHPLevel;
    char _domeHPRegenLevel;
    float _regenRate;

    float _hp;
    float _maxHP;

    DomeManager();
};
