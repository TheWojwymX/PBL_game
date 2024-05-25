//
// Created by Jacek on 25.05.2024.
//

#include "HUD/Pages/DialogPage/DialogPage.h"

#define TUTORIALMANAGER TutorialManager::getInstance()

#ifndef SANDBOX_TUTORIALMANAGER_H
#define SANDBOX_TUTORIALMANAGER_H

class TutorialManager {
public:
    static TutorialManager &getInstance();
    ~TutorialManager() = default;
    TutorialManager() = default;
    TutorialManager(const TutorialManager &) = delete;
    TutorialManager &operator=(const TutorialManager &) = delete;

    shared_ptr<DialogPage> _dialogPage;

    void Init();
    void Update();

    string _dialog1 = "Lorem ipsum dolor sit amet, consectetur adipiscing elit,\n"
                      "sed do eiusmod tempor incididunt ut labore et dolore\n"
                      "magna aliqua. Ut enim ad minim veniam, quis nostrud\n"
                      "exercitation ullamco laboris nisi ut aliquip ex ea\n"
                      "commodo consequat. Duis aute irure dolor in\n"
                      "reprehenderit in voluptate velit esse cillum dolore eu\n"
                      "fugiat nulla pariatur. Excepteur sint occaecat cupidatat\n"
                      "non proident, sunt in culpa qui officia deserunt mollit\n"
                      "anim id est laborum.";

};


#endif //SANDBOX_TUTORIALMANAGER_H
