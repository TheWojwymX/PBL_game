#include "DomeManager.h"

DomeManager& DomeManager::getInstance() {
    static DomeManager instance;
    return instance;
}

DomeManager::DomeManager()
    : _domeHPLevel(0), _domeHPRegenLevel(0), _regenRate(0.1f), _hp(100.0f), _maxHP(100.0f)
{
}

void DomeManager::TakeDamage(float value) {
    _hp -= value;
    std::cout << "Dome HP: " << _hp << "/" << _maxHP << " value: "<< value<< std::endl;
}

void DomeManager::RegenHP() {
    _hp += _regenRate * TIME.GetDeltaTime();
    if (_hp > _maxHP) {
        _hp = _maxHP;
    }
    //std::cout << "Regen Dome HP: " << _hp << "/" << _maxHP << std::endl;
}

void DomeManager::Update() {
    RegenHP();
}

void DomeManager::RepairDomeHP(float value) {
    _hp += value;
    if (_hp > _maxHP) {
        _hp = _maxHP;
    }
    std::cout << "Dome HP after repair: " << _hp << "/" << _maxHP << std::endl;
}

void DomeManager::Reset() {
    _domeHPLevel = 0;
    _domeHPRegenLevel = 0;
    _regenRate = 0.1f;
    _hp = 100.0f;
    _maxHP = 100.0f;
}
