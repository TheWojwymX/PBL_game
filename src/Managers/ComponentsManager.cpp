//
// Created by Jacek on 23.03.2024.
//

#include "ComponentsManager.h"

ComponentsManager& ComponentsManager::getInstance() {
    static ComponentsManager instance;
    return instance;
}
