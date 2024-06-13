#include "DomeManager.h"

DomeManager::DomeManager()
    : _domeHPLevel(0),
    _domeHPRegenLevel(0),
    _regenRate(0.0f),
    _hp(100),
    _maxHP(100)
{}

DomeManager& DomeManager::getInstance() {
    static DomeManager instance;
    return instance;
}

void DomeManager::TakeDamage(float value) {
    _hp -= value;
    if (_hp <= 0) {
        std::cout << "Game Over!" << std::endl;
        // Additional game over logic or state handling
        return;
    }
}

void DomeManager::Update() {
    RegenHP();
}

void DomeManager::RegenHP() {
    _hp = glm::min(_maxHP, _hp + (_regenRate * TIME.GetDeltaTime()));
}
