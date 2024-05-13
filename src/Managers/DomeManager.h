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
    float iTime;
    bool isInvincible;

private:
    DomeManager() : hp(200),iTime(0),isInvincible(false) {}


};
