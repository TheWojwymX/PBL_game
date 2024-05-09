//
// Created by Jacek on 09.05.2024.
//

#define PAGEMANAGER PageManager::getInstance()

#ifndef SANDBOX_PAGEMANAGER_H
#define SANDBOX_PAGEMANAGER_H

#include <vector>
#include <memory>
#include "HUD/Pages/Page.h"
#include "HUD/Pages/PauseMenuPage.h"


class PageManager {
public:

    ~PageManager() = default;

    PageManager() = default;

    static PageManager &getInstance();

    PageManager(const PageManager &) = delete;

    PageManager &operator=(const PageManager &) = delete;

    void Init();
    void Update();

    std::vector<std::shared_ptr<Page>> _pages;
    PauseMenuPage _pauseMenuPage;

};


#endif //SANDBOX_PAGEMANAGER_H
