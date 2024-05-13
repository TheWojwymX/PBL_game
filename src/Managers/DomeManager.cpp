#include "DomeManager.h"
#include "Core/Time.h"

DomeManager &DomeManager::getInstance() {
    static DomeManager instance;
    return instance;
}

void DomeManager::takeDamage(int value)
{
    if(!isInvincible)
    {
        isInvincible = true;
        hp = hp - value;
    }
}

void DomeManager::Update()
{
    if (hp <= 0.01)
    {
        //End the game
    }
    // This might be changed to powerup
    //that makes base invincible for few secs.
    if(iTime < 3.0f)
    {
        iTime += TIME.GetDeltaTime();
    }
    else
    {
        isInvincible = false;
        iTime = 0.0f;
    }
}
