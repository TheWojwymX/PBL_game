#pragma once

#define DOMEMANAGER DomeManager::getInstance()

class DomeManager {
public:
    static DomeManager &getInstance();
    ~DomeManager() = default;
    DomeManager(const DomeManager &) = delete;
    DomeManager &operator=(const DomeManager &) = delete;
    void takeDamage(int value);
    void Update();
    char GetDomeHPLevel() { return _domeHPLevel; }
    void UpgradeDomeHP(float value) { maxHP+=value; _domeHPLevel++; }

    int hp;
    int maxHP = 200;

private:
    char _domeHPLevel = 0;
    DomeManager() : hp(200) {}
};
