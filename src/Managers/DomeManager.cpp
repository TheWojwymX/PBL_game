#include "DomeManager.h"
#include "Core/Time.h"

DomeManager &DomeManager::getInstance() {
    static DomeManager instance;
    return instance;
}

void DomeManager::takeDamage(int value)
{
     hp = hp - value;
}

void DomeManager::Update()
{
    if (hp <= 0.01)
    {
        //End the game
    }

}
