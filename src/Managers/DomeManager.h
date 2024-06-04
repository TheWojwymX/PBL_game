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

    int hp;
    int maxHP = 200;

private:
    DomeManager() : hp(200) {}


};
