//
// Created by Jacek on 25.03.2024.
//

#include "NodesManager.h"

NodesManager& NodesManager::getInstance() {
    static NodesManager instance;
    return instance;
}